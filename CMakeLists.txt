cmake_minimum_required(VERSION 3.20)

set(TARGET game)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(
    ${TARGET}
    VERSION 0.1.0
    DESCRIPTION "game from scratch"
    HOMEPAGE_URL "https://github.com/m1dsolo/game"
    LANGUAGES C CXX
)

set(WHEEL_ROOT ${PROJECT_SOURCE_DIR}/third_party/wheel)
set(SDL_ROOT ${PROJECT_SOURCE_DIR}/third_party/SDL)
set(SDL_IMAGE ${PROJECT_SOURCE_DIR}/third_party/SDL_image)
set(SDL_MIXER ${PROJECT_SOURCE_DIR}/third_party/SDL_mixer)
set(SDL_TTF ${PROJECT_SOURCE_DIR}/third_party/SDL_ttf)
set(PerlinNoise ${PROJECT_SOURCE_DIR}/third_party/PerlinNoise)

add_subdirectory(${WHEEL_ROOT} wheel.out)
add_subdirectory(${SDL_ROOT} SDL.out)
add_subdirectory(${SDL_IMAGE} SDL_image.out)
add_subdirectory(${SDL_MIXER} SDL_mixer.out)
add_subdirectory(${SDL_TTF} SDL_ttf.out)
# add_subdirectory(server)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "src/*.cpp")
add_executable(${TARGET} ${SRC})
target_include_directories(${TARGET}
    PRIVATE include
    PRIVATE ${WHEEL_ROOT}/include
    PRIVATE ${SDL_ROOT}/include
    PRIVATE ${SDL_IMAGE}/include 
    PRIVATE ${SDL_MIXER}/include 
    PRIVATE ${SDL_TTF}/include 
    PRIVATE ${PerlinNoise}
)
target_link_libraries(${TARGET}
    PRIVATE wheel
    PRIVATE SDL3::SDL3
    PRIVATE SDL3_image::SDL3_image
    PRIVATE SDL3_mixer::SDL3_mixer
    PRIVATE SDL3_ttf::SDL3_ttf
)
