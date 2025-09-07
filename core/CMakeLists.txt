cmake_minimum_required(VERSION 3.16)

set(TARGET core)

set(WHEEL_ROOT third_party/wheel)
set(ECS_ROOT third_party/ecs)
set(SDL_ROOT third_party/sdl)
set(REFLECT_ROOT third_party/reflect-cpp)

add_subdirectory(${WHEEL_ROOT})
add_subdirectory(${ECS_ROOT})
add_subdirectory(${SDL_ROOT})
add_subdirectory(${REFLECT_ROOT})

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "src/*.cpp")

add_library(${TARGET} STATIC ${SRC})

target_include_directories(${TARGET}
    PUBLIC include
    PUBLIC ${WHEEL_ROOT}/include
    PUBLIC ${ECS_ROOT}/include
    PUBLIC ${REFLECT_ROOT}/include
)
target_link_libraries(${TARGET}
    PUBLIC wheel
    PUBLIC ecs
    PUBLIC sdl
    PUBLIC reflectcpp
)
