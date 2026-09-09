cmake_minimum_required(VERSION 3.24)
project(pkgvault VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dependencies
find_package(nlohmann_json 3.11 QUIET)

set(JSONLIB_VERSION 3.12.0)
set(JSONLIB_GIT_URL https://github.com/nlohmann/json/releases/download/v${JSONLIB_VERSION}/json.tar.xz)

if(NOT nlohmann_json_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            json
            URL ${JSONLIB_GIT_URL}
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_GetProperties(json)
    if(NOT json_POPULATED)
        FetchContent_Populate(json)
        add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
endif()

add_executable(pkgvault
        main.cpp
        pacmans.cpp
        exporters/json_exporter.cpp
)

target_include_directories(pkgvault PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(TARGET nlohmann_json::nlohmann_json)
    target_link_libraries(pkgvault PRIVATE nlohmann_json::nlohmann_json)
else()
    target_link_libraries(pkgvault PRIVATE nlohmann_json)
endif()

include(GNUInstallDirs)

install(TARGETS pkgvault
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# CPack conf
set(CPACK_PACKAGE_NAME "pkgvault")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_MAINTAINER "Abdulloh Raupov <abdullohraupov.work@gmail.com>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A minimalist CLI tool to save and restore installed packages across different Linux distributions.")
set(CPACK_PACKAGE_VENDOR "pkgvault")

set(CPACK_GENERATOR "DEB;RPM")

# deb package
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_MAINTAINER}")
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)

# rpm package
set(CPACK_RPM_PACKAGE_LICENSE "GPLv3")
set(CPACK_RPM_PACKAGE_GROUP "Applications/System")
set(CPACK_RPM_PACKAGE_AUTOREQPROV ON)
set(CPACK_RPM_FILE_NAME RPM-DEFAULT)

include(CPack)
