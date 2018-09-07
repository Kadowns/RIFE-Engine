cmake_minimum_required(VERSION 3.11)
project(RIFE)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_HOME_DIRECTORY}/cmake)

include_directories(${Vulkan_INCLUDE_DIR})
find_package(Vulkan REQUIRED)
find_package(GLFW REQUIRED)

add_library(Vulkan SHARED IMPORTED)

add_executable(RIFE ${CMAKE_HOME_DIRECTORY}/Engine/main.cpp)
target_link_libraries(RIFE Vulkan::Vulkan GLFW)

target_sources(RIFE PUBLIC 
    ${CMAKE_HOME_DIRECTORY}/Engine/Keyboard.cpp
    ${CMAKE_HOME_DIRECTORY}/Engine/Window.cpp
    ${CMAKE_HOME_DIRECTORY}/Engine/Triangle.cpp
    ${CMAKE_HOME_DIRECTORY}/Engine/Test.cpp
    ${CMAKE_HOME_DIRECTORY}/Engine/Time.cpp
)

