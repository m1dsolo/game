cmake_minimum_required(VERSION 3.16)

set(TARGET survivor)

project(
    survivor
    VERSION 0.1.0
    DESCRIPTION "survivor"
    # HOMEPAGE_URL "https://github.com/m1dsolo/survivor"
    LANGUAGES C CXX
)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "src/*.cpp")
add_executable(${TARGET} ${SRC})

target_include_directories(${TARGET}
    PRIVATE include
)
target_link_libraries(${TARGET}
    PRIVATE core
)
