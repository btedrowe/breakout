cmake_minimum_required(VERSION 3.10)
project(breakout VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)

find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_subdirectory(src)
add_subdirectory(assets)

add_executable(breakout ${BREAKOUT_SOURCES})
add_dependencies(breakout copy_assets)
target_include_directories(breakout PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries(breakout PRIVATE SDL2)
target_link_libraries(breakout PRIVATE glm::glm)
target_link_libraries(breakout PRIVATE GLEW)
target_link_libraries(breakout PRIVATE OpenGL)
