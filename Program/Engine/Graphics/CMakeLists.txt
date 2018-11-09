include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

find_package(Vulkan REQUIRED)
find_package(GLFW REQUIRED)

set(GRAPHICS_INCLUDE
	Include/VulkanInclude.h
	Include/VulkanBase.h 
	Include/VulkanTools.h
	Include/VulkanData.h
	Include/Vertex.h
	Include/Renderer.h
	Include/tiny_obj_loader.h
	Include/Mesh.h
	Include/MeshBuilder.h
	Include/MeshFactory.h
	Include/MeshRenderer.h
	Include/Material.h
	Include/MaterialBuilder.h
	Include/MaterialFactory.h
	Include/MaterialInstance.h
	Include/Shader.h
	Include/ShaderBuilder.h
	Include/ShaderFactory.h
	Include/ShaderItem.h
	Include/Transform.h
	Include/stb_image.h
	Include/Texture.h
	Include/TextureFactory.h
	Include/Camera.h
	Include/Lights.h
	Include/GlobalLights.h
	Include/UniformBufferObject.h	 
	Include/RifeGraphics.h
	)

set(GRAPHICS_SOURCE
	Source/VulkanBase.cpp
	Source/VulkanTools.cpp
	Source/VulkanData.cpp
	Source/Vertex.cpp 
	Source/Renderer.cpp
	Source/Mesh.cpp
	Source/MeshBuilder.cpp
	Source/MeshFactory.cpp
	Source/MeshRenderer.cpp
	Source/Material.cpp
	Source/MaterialBuilder.cpp
	Source/MaterialFactory.cpp
	Source/MaterialInstance.cpp
	Source/Shader.cpp
	Source/ShaderBuilder.cpp
	Source/ShaderFactory.cpp
	Source/Texture.cpp
	Source/TextureFactory.cpp
	Source/Camera.cpp
	Source/Lights.cpp
	Source/GlobalLights.cpp
	)

set(GRAPHICS_ALL "${GRAPHICS_INCLUDE}" "${GRAPHICS_SOURCE}")

CreateModule(Engine Graphics "${GRAPHICS_ALL}")

target_link_libraries(Graphics GLFW Vulkan::Vulkan Base)
