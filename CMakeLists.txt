cmake_minimum_required(VERSION 3.24)
project(pkgvault VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(nlohmann_json 3.11 QUIET)

if(NOT nlohmann_json_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            json
            URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(json)
endif()

add_executable(pkgvault
        main.cpp
        pacmans.cpp
        exporters/json_exporter.cpp
)

target_include_directories(pkgvault PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(pkgvault PRIVATE nlohmann_json::nlohmann_json)

install(TARGETS pkgvault DESTINATION bin)