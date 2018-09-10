cmake_minimum_required(VERSION 3.11)
project(RIFE)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_HOME_DIRECTORY}/cmake)

add_subdirectory(Engine)
add_subdirectory(Example)
#
#set(GENERAL_SOURCES
#             ${MAIN_DEFINITION}
#			 ${TOOLS_DEFINITION}
#			 ${GRAPHICS_DEFINITION}
#			 ${APPLICATION_DEFINITION})
#
#message("Creating executable: ${GENERAL_SOURCES}")
#add_executable(RIFE Engine/Application/main.cpp)
#target_sources(RIFE PUBLIC ${GENERAL_SOURCES})
#target_include_directories(RIFE PUBLIC ${Vulkan_INCLUDE_DIR})
#
#
#
