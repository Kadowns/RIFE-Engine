cmake_minimum_required(VERSION 3.11)
project(RIFE)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_HOME_DIRECTORY}/cmake)

add_subdirectory(Engine)
add_subdirectory(Core)