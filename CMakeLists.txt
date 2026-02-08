cmake_minimum_required(VERSION 3.15)

project(mystl)

# Version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for Neovim Lsp client
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build type
set(CMAKE_BUILD_TYPE release)

# TODO: Add Googletest

add_subdirectory(${PROJECT_SOURCE_DIR}/test)
