#include "json_exporter.hpp"
#include <nlohmann/json.hpp>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>

void json_exporter::save(const std::vector<package> &pkgs, const std::filesystem::path &path)
{
    nlohmann::json j;
    j["packages"] = nlohmann::json::array();

    for (const auto &pkg : pkgs)
    {
        j["packages"].push_back(pkg.name);
    }

    std::ofstream file(path);
    if (file.is_open())
    {
        file << j.dump(4);

        if (!file)
        {
            std::cerr << "Error: Failed to write data to " << path << std::endl;
        }

        std::cout << "Successfully saved " << pkgs.size() << " packages to " << path << std::endl;
    }
    else
    {
        std::cerr << "Error: Could not open file " << path << " for writing." << std::endl;
    }
}

std::vector<package> json_exporter::load(const std::filesystem::path &path)
{
    std::vector<package> pkgs;
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << path << " for reading." << std::endl;
        return pkgs;
    }

    try
    {
        nlohmann::json j = nlohmann::json::parse(file);
        if (j.contains("packages") && j["packages"].is_array())
        {
            for (const auto &name : j["packages"])
            {
                pkgs.push_back({name.get<std::string>()});
            }
        }
    }
    catch (const nlohmann::json::parse_error& e)
    {
        std::cerr << "Error: Failed to parse JSON file: " << e.what() << std::endl;
    }

    return pkgs;
}
