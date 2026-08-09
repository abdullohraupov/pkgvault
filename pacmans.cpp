#include "pacmans.hpp"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <string_view>
#include <vector>

#include <unistd.h>

namespace
{
    constexpr std::string_view RESET = "\033[0m";
    constexpr std::string_view RED   = "\033[31m";

    bool is_valid_pkg_name(const std::string& name)
    {
        if (name.empty() || name.length() > 128) return false;

        return std::all_of(name.begin(), name.end(), [](unsigned char c) {
            return std::isalnum(c) || c == '_' || c == '.' || c == '+' || c == '-' || c == '@';
        });
    }

    std::vector<std::string> exec(const char* cmd)
    {
        std::vector<std::string> output;
        FILE* pipe = popen(cmd, "r");
        if (!pipe) return output;

        char buffer[512];
        while (fgets(buffer, sizeof(buffer), pipe))
        {
            std::string line = buffer;
            line.erase(line.find_last_not_of("\r\n") + 1);

            if (!line.empty())
            {
                output.push_back(line);
            }
        }

        pclose(pipe);
        return output;
    }

    void install(const char* base_cmd, const std::vector<package>& pkgs)
    {
        if (pkgs.empty()) return;

        std::string cmd = (geteuid() == 0) ? "" : "sudo ";
        cmd += base_cmd;

        for (const auto& [name] : pkgs)
        {
            if (!is_valid_pkg_name(name))
            {
                std::cerr << RED << "Warning: " << RESET << "Skipping invalid package name: " << name << "\n";
                continue;
            }

            cmd += " " + name;
        }

        system(cmd.c_str());
    }

    std::vector<package> save_simple_pkgs(const char* cmd)
    {
        std::vector<package> pkgs;
        const auto lines = exec(cmd);

        for (const auto& line : lines)
        {
            if (is_valid_pkg_name(line))
            {
                pkgs.push_back({line});
            }
        }

        return pkgs;
    }
} // namespace


// APT

void apt::installPkgs(const std::vector<package>& pkgs)
{
    install("apt install -y", pkgs);
}

std::vector<package> apt::savePkgs()
{
    return save_simple_pkgs("apt-mark showmanual 2>/dev/null");
}


// DNF

void dnf::installPkgs(const std::vector<package>& pkgs)
{
    install("dnf install -y", pkgs);
}

std::vector<package> dnf::savePkgs()
{
    return save_simple_pkgs("rpm -qa --qf '%{NAME}\n' 2>/dev/null");
}


// PACMAN

void pacman::installPkgs(const std::vector<package>& pkgs)
{
    install("pacman -S --needed --noconfirm", pkgs);
}

std::vector<package> pacman::savePkgs()
{
    return save_simple_pkgs("pacman -Qeq 2>/dev/null");
}


// ZYPPER

void zypper::installPkgs(const std::vector<package>& pkgs)
{
    install("zypper install -y", pkgs);
}

std::vector<package> zypper::savePkgs()
{
    return save_simple_pkgs("rpm -qa --qf '%{NAME}\n' 2>/dev/null");
}


// XBPS

void xbps::installPkgs(const std::vector<package>& pkgs)
{
    install("xbps-install -y", pkgs);
}

std::vector<package> xbps::savePkgs()
{
    std::vector<package> pkgs;
    const auto lines = exec("xbps-query -m 2>/dev/null");

    for (const auto& line : lines)
    {
        if (const size_t last_dash = line.rfind('-'); last_dash != std::string::npos)
        {
            if (std::string name = line.substr(0, last_dash); is_valid_pkg_name(name))
            {
                pkgs.push_back({name});
            }
        }
    }

    return pkgs;
}