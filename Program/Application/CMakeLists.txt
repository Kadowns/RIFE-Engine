add_subdirectory(Common)

add_executable(RIFE_Engine main.cpp)
target_link_libraries(RIFE_Engine Common)