include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

find_package(Vulkan REQUIRED)
find_package(GLFW REQUIRED)

set(GRAPHICS_INCLUDE 
	Include/Vertex.h
	Include/VkUtilities.h 
	Include/UniformBufferObject.h
	Include/GraphicsDependencys.h 
	Include/GraphicsIncludes.h)
set(GRAPHICS_SOURCE Source/Vertex.cpp Source/VkUtilities.cpp)
set(GRAPHICS_ALL "${GRAPHICS_INCLUDE}" "${GRAPHICS_SOURCE}")
CreateModule(Graphics "${GRAPHICS_ALL}")


target_link_libraries(Graphics GLFW Vulkan::Vulkan Utils)
