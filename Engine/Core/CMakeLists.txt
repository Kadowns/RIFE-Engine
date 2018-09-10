include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)
#include(${CMAKE_HOME_DIRECTORY}/Engine/CMakeLists.txt)

set(MAIN_INCLUDE Main/Window.h Main/Scene.h)
set(MAIN_DEFINITION Main/main.cpp Main/Window.cpp)
set(MAIN_ALL ${MAIN_INCLUDE} ${MAIN_DEFINITION})
CreateModule(Main ${MAIN_ALL})

set(TOOLS_INCLUDE Tools/Keyboard.h Tools/Time.h)
set(TOOLS_DEFINITION Tools/Keyboard.cpp Tools/Keyboard.h)
set(TOOLS_ALL ${TOOLS_INCLUDE} ${TOOLS_DEFINITION})
CreateModule(Tools ${TOOLS_ALL})


find_package(Vulkan REQUIRED)
find_package(GLFW REQUIRED)
set(GLFW_DIR ${CMAKE_HOME_DIRECTORY}/libs/glfw)
set(GLFW_INCLUDE_DIR ${GLFW_DIR}/include)
set(GLFW_LIBRARY ${GLFW_DIR}/lib)

