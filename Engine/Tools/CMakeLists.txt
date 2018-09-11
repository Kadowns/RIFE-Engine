include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

set(TOOLS_GENERAL Keyboard.h Clock.h Keyboard.cpp Clock.cpp)
set(TOOLS_ALL ${TOOLS_GENERAL})
CreateModule(Tools "${TOOLS_ALL}")
find_package(Vulkan REQUIRED)
find_package(GLFW REQUIRED)
target_link_libraries(Tools GLFW Vulkan::Vulkan)