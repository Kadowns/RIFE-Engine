add_subdirectory(components)
add_subdirectory(scene)


add_executable(RIFE_Engine main.cpp)
target_link_libraries(RIFE_Engine scene)