include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

find_package(Vulkan REQUIRED)
find_package(GLFW REQUIRED)

set(GRAPHICS_INCLUDE 
	Include/Camera.h
	Include/Vertex.h
	Include/Renderer.h
	Include/Mesh.h
	Include/MeshRenderer.h
	Include/VkUtilities.h 
	Include/Shader.h
	Include/GraphicsPipeline.h
	Include/UniformBufferObject.h
	Include/GraphicsDependencys.h 
	Include/RifeGraphics.h)

set(GRAPHICS_SOURCE
	Source/Vertex.cpp 
	Source/Mesh.cpp
	Source/Camera.cpp
	Source/MeshRenderer.cpp
	Source/VkUtilities.cpp
	Source/Shader.cpp
	Source/GraphicsPipeline.cpp)

set(GRAPHICS_ALL "${GRAPHICS_INCLUDE}" "${GRAPHICS_SOURCE}")

CreateModule(Engine Graphics "${GRAPHICS_ALL}")

target_link_libraries(Graphics GLFW Vulkan::Vulkan Base)
