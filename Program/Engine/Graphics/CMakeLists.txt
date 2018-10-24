include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

find_package(Vulkan REQUIRED)
find_package(GLFW REQUIRED)

set(GRAPHICS_INCLUDE 
	Include/Vertex.h
	Include/Renderer.h
	Include/Mesh.h
	Include/MeshRenderer.h
	Include/Material.h
	Include/MaterialBuilder.h
	Include/MaterialFactory.h
	Include/VkUtilities.h 
	Include/UniformBufferObject.h
	Include/GraphicsLibs.h 
	Include/RifeGraphics.h)

set(GRAPHICS_SOURCE
	Source/Vertex.cpp 
	Source/Mesh.cpp
	Source/MeshRenderer.cpp
	Source/Material.cpp
	Source/MaterialBuilder.cpp
	Source/MaterialFactory.cpp
	Source/VkUtilities.cpp
	)

set(GRAPHICS_ALL "${GRAPHICS_INCLUDE}" "${GRAPHICS_SOURCE}")

CreateModule(Engine Graphics "${GRAPHICS_ALL}")

target_link_libraries(Graphics GLFW Vulkan::Vulkan Base)
