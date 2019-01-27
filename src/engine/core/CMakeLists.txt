include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

message("Creating core")

add_subdirectory(math)
add_subdirectory(base)
