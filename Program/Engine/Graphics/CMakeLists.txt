include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

find_package(Vulkan REQUIRED)
find_package(GLFW REQUIRED)

set(GRAPHICS_INCLUDE Include/VkUtilities.h)
set(GRAPHICS_SOURCE Source/VkUtilities.cpp)
set(GRAPHICS_ALL ${GRAPHICS_INCLUDE} ${GRAPHICS_SOURCE})
CreateModule(Graphics "${GRAPHICS_ALL}")


target_link_libraries(Graphics GLFW Vulkan::Vulkan Utils)
