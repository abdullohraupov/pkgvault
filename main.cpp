#include "pacmans.hpp"
#include "exporters/json_exporter.hpp"

#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <memory>

const std::string version = "1.0";

constexpr inline std::string_view RESET = "\033[0m";
constexpr inline std::string_view GREEN = "\033[32m";
constexpr inline std::string_view BLUE  = "\033[34m";
constexpr inline std::string_view RED   = "\033[31m";
constexpr inline std::string_view BOLD  = "\033[1m";

void print_help() {
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

struct PMInfo {
    std::unique_ptr<backend> pm;
    std::string name;
};

PMInfo detect_pm() {
    if (std::filesystem::exists("/usr/bin/apt")) return {std::make_unique<apt>(), "apt"};
    if (std::filesystem::exists("/usr/bin/dnf")) return {std::make_unique<dnf>(), "dnf"};
    if (std::filesystem::exists("/usr/bin/pacman")) return {std::make_unique<pacman>(), "pacman"};
    if (std::filesystem::exists("/usr/bin/zypper")) return {std::make_unique<zypper>(), "zypper"};
    if (std::filesystem::exists("/usr/bin/xbps")) return {std::make_unique<xbps>(), "xbps"};
    return {nullptr, ""};
}

int main(int argc, char** argv) {
    if (argc < 2) {
        print_help();
        return 0;
    }

    auto [pm, pm_name] = detect_pm();
    if (!pm) {
        std::cerr << RED << "Error:" << RESET << " No supported package manager detected.\n";
        return 1;
    }

    std::string arg = argv[1];

    if (arg == "-h" || arg == "--help") {
        print_help();
    } else if (arg == "-v" || arg == "--version") {
        std::cout << "pkgvault version " << BOLD << version << RESET << "\n";
    } else if (arg == "-s" || arg == "--save") {
        if (argc < 3) {
            std::cerr << RED << "Error:" << RESET << " Missing file path for --save\n";
            return 1;
        }
        std::cout << BLUE << "::" << RESET << " Detected package manager: " << BOLD << pm_name << RESET << "\n";
        std::cout << BLUE << "::" << RESET << " Scanning for installed packages...\n";
        auto pkgs = pm->savePkgs();
        json_exporter exp;
        exp.save(pkgs, argv[2]);
    } else if (arg == "-i" || arg == "--install") {
        if (argc < 3) {
            std::cerr << RED << "Error:" << RESET << " Missing file path for --install\n";
            return 1;
        }
        std::cout << BLUE << "::" << RESET << " Detected package manager: " << BOLD << pm_name << RESET << "\n";
        std::cout << BLUE << "::" << RESET << " Loading package list from " << BOLD << argv[2] << RESET << "...\n";
        json_exporter exp;
        auto pkgs = exp.load(argv[2]);
        if (pkgs.empty()) {
            std::cerr << RED << "Error:" << RESET << " No packages found or file could not be read.\n";
            return 1;
        }
        std::cout << BLUE << "::" << RESET << " Found " << BOLD << pkgs.size() << RESET << " packages. Starting installation...\n";
        pm->installPkgs(pkgs);
        std::cout << GREEN << "Done!" << RESET << "\n";
    } else {
        std::cerr << RED << "Error:" << RESET << " Unknown option '" << arg << "'\n";
        print_help();
        return 1;
    }

    return 0;
}
