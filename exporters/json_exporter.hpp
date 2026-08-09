#pragma once
#include "../package.hpp"
#include <vector>
#include <filesystem>

class json_exporter
{
public:
    static void save(const std::vector<package> &pkgs, const std::filesystem::path &path);

    static std::vector<package> load(const std::filesystem::path &path);
};
