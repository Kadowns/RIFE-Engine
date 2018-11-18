add_subdirectory(Assets)
add_subdirectory(Scene)


add_executable(RIFE_Engine main.cpp)
target_link_libraries(RIFE_Engine Scene)