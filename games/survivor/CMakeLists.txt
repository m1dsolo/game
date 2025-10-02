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

add_custom_command(TARGET survivor POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/core/assets
          $<TARGET_FILE_DIR:survivor>/assets
  
  DEPENDS ${CMAKE_SOURCE_DIR}/core/assets
  
  COMMENT "Updating core assets in build directory"
)

# 复制 survivor 自身的 assets 到构建目录
add_custom_command(TARGET survivor POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/assets  # 源目录
          $<TARGET_FILE_DIR:survivor>/assets  # 目标目录
  
  # 监听自身 assets 目录变化
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/assets
  
  COMMENT "Updating survivor-specific assets in build directory"
)
