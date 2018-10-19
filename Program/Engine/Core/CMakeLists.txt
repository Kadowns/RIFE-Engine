include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

message("Creating core")

add_subdirectory(Math)
add_subdirectory(Base)
