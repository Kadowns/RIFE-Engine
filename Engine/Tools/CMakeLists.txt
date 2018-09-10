include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

set(TOOLS_GENERAL Keyboard.h Time.h Keyboard.cpp Time.cpp)
set(TOOLS_ALL ${TOOLS_GENERAL})
CreateModule(Tools "${TOOLS_ALL}")
find_package(GLFW REQUIRED)
target_link_libraries(Tools GLFW)