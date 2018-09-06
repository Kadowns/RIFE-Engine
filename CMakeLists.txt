cmake_minimum_required(VERSION 3.11)
project(RIFE_Engine)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_HOME_DIRECTORY}/cmake)
find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)
add_executable(RIFE_Engine ${CMAKE_HOME_DIRECTORY}/Engine/main.cpp)
target_link_libraries(RIFE_Engine GLEW GLFW)