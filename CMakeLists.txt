cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(
    game
    VERSION 0.1.0
    DESCRIPTION "game"
    HOMEPAGE_URL "https://github.com/m1dsolo/game"
    LANGUAGES C CXX
)

add_subdirectory(${PROJECT_SOURCE_DIR}/core)
add_subdirectory(${PROJECT_SOURCE_DIR}/games)
