#include "exporters/json_exporter.hpp"
#include "pacmans.hpp"

#include <cstdlib>
#include <exception>
#include <filesystem>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace fs = std::filesystem;

constexpr std::string_view version = "1.0";

constexpr std::string_view RESET = "\033[0m";
constexpr std::string_view GREEN = "\033[32m";
constexpr std::string_view BLUE = "\033[34m";
constexpr std::string_view RED = "\033[31m";
constexpr std::string_view BOLD = "\033[1m";

static void print_help()
{
    std::cout << BOLD << BLUE << "PkgVault v" << version << RESET << " - Securely manage your package lists\n\n";
    std::cout << BOLD << "Usage:" << RESET << "\n";
    std::cout << "  pkgvault [options]\n\n";
    std::cout << BOLD << "Options:" << RESET << "\n";
    std::cout << "  -s, --save <file>      " << GREEN << "Save" << RESET << " current list of installed packages to <file>\n";
    std::cout << "  -i, --install <file>   " << GREEN << "Install" << RESET << " packages from a saved <file>\n";
    std::cout << "  -v, --version          Show version information\n";
    std::cout << "  -h, --help             Show this help message\n\n";
    std::cout << BOLD << "Supported Package Managers:" << RESET << " apt, dnf, pacman, zypper, xbps\n";
}

namespace
{
    struct PMInfo
    {
        std::unique_ptr<backend> pm;
        std::string name;
    };

    bool has_executable(const std::string &exec_name)
    {
        const char *path_env = std::getenv("PATH");
        std::vector<fs::path> search_paths;

        if (path_env)
        {
            std::stringstream ss(path_env);
            std::string path_str;
            while (std::getline(ss, path_str, ':'))
            {
                if (!path_str.empty())
                    search_paths.emplace_back(path_str);
            }
        }

        search_paths.insert(search_paths.end(), {
            "/usr/bin", "/bin", "/usr/local/bin", "/usr/sbin", "/sbin"
        });

        for (const auto &dir : search_paths)
        {
            std::error_code ec;
            if (auto full_path = dir / exec_name; fs::exists(full_path, ec) && fs::is_regular_file(full_path, ec))
            {
                return true;
            }
        }
        return false;
    }
} // namespace

static PMInfo detect_pm()
{
    if (has_executable("apt"))
        return {.pm = std::make_unique<apt>(), .name = "apt"};
    if (has_executable("dnf"))
        return {.pm = std::make_unique<dnf>(), .name = "dnf"};
    if (has_executable("pacman"))
        return {.pm = std::make_unique<pacman>(), .name = "pacman"};
    if (has_executable("zypper"))
        return {.pm = std::make_unique<zypper>(), .name = "zypper"};
    if (has_executable("xbps-install") || has_executable("xbps"))
        return {.pm = std::make_unique<xbps>(), .name = "xbps"};

    return {.pm = nullptr, .name = ""};
}

int main(const int argc, char **argv)
{
    if (argc < 2)
    {
        print_help();
        return 0;
    }

    const std::string arg = argv[1];

    if (arg == "-h" || arg == "--help")
    {
        print_help();
        return 0;
    }

    if (arg == "-v" || arg == "--version")
    {
        std::cout << "pkgvault version " << BOLD << version << RESET << "\n";
        return 0;
    }

    if (arg == "-s" || arg == "--save")
    {
        if (argc < 3)
        {
            std::cerr << RED << "Error:" << RESET << " Missing file path for --save\n";
            return 1;
        }

        auto [pm, pm_name] = detect_pm();
        if (!pm)
        {
            std::cerr << RED << "Error:" << RESET << " No supported package manager detected.\n";
            return 1;
        }

        try
        {
            std::cout << BLUE << "::" << RESET << " Detected package manager: " << BOLD << pm_name << RESET << "\n";
            std::cout << BLUE << "::" << RESET << " Scanning for installed packages...\n";

            const auto pkgs = pm->savePkgs();
            json_exporter::save(pkgs, argv[2]);
        }
        catch (const std::exception &e)
        {
            std::cerr << RED << "Error:" << RESET << " Failed to save package list: " << e.what() << "\n";
            return 1;
        }

        return 0;
    }

    if (arg == "-i" || arg == "--install")
    {
        if (argc < 3)
        {
            std::cerr << RED << "Error:" << RESET << " Missing file path for --install\n";
            return 1;
        }

        auto [pm, pm_name] = detect_pm();
        if (!pm)
        {
            std::cerr << RED << "Error:" << RESET << " No supported package manager detected.\n";
            return 1;
        }

        try
        {
            std::cout << BLUE << "::" << RESET << " Detected package manager: " << BOLD << pm_name << RESET << "\n";
            std::cout << BLUE << "::" << RESET << " Loading package list from " << BOLD << argv[2] << RESET << "...\n";

            const auto pkgs = json_exporter::load(argv[2]);

            if (pkgs.empty())
            {
                std::cerr << RED << "Error:" << RESET << " No packages found or file is empty.\n";
                return 1;
            }

            std::cout << BLUE << "::" << RESET << " Found " << BOLD << pkgs.size() << RESET << " packages. Starting installation...\n";
            pm->installPkgs(pkgs);

            std::cout << GREEN << "Done!" << RESET << "\n";
        }
        catch (const std::exception &e)
        {
            std::cerr << RED << "Error:" << RESET << " Failed to install packages: " << e.what() << "\n";
            return 1;
        }

        return 0;
    }

    std::cerr << RED << "Error:" << RESET << " Unknown option '" << arg << "'\n";
    print_help();

    return 1;
}