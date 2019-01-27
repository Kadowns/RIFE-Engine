include(${CMAKE_HOME_DIRECTORY}/cmake/RIFECreateModule.cmake)

find_package(Vulkan REQUIRED)
find_package(GLFW REQUIRED)
find_package(GLI REQUIRED)

set(GRAPHICS_INCLUDE
	include/VulkanInclude.h
	include/VulkanBase.h 
	include/VulkanTools.h
	include/VulkanData.h
	include/Vertex.h
	include/Buffer.h
	include/Renderer.h
	include/tiny_obj_loader.h
	include/Mesh.h
	include/MeshBuilder.h
	include/MeshFactory.h
	include/MeshRenderer.h
	include/Material.h
	include/MaterialBuilder.h
	include/MaterialFactory.h
	include/MaterialInstance.h
	include/Shader.h
	include/ShaderBuilder.h
	include/ShaderFactory.h
	include/ShaderItem.h
	include/Skybox.h
	include/SkyboxRenderer.h
	include/Transform.h
	include/Texture.h
	include/TextureFactory.h
	include/Camera.h
	include/Lights.h
	include/GlobalLights.h
	include/UniformBufferObject.h	 
	include/RifeGraphics.h
	)

set(GRAPHICS_SOURCE
	source/VulkanBase.cpp
	source/VulkanTools.cpp
	source/VulkanData.cpp
	source/Vertex.cpp 
	source/Buffer.cpp
	source/Renderer.cpp
	source/Mesh.cpp
	source/MeshBuilder.cpp
	source/MeshFactory.cpp
	source/MeshRenderer.cpp
	source/Material.cpp
	source/MaterialBuilder.cpp
	source/MaterialFactory.cpp
	source/MaterialInstance.cpp
	source/Shader.cpp
	source/ShaderBuilder.cpp
	source/ShaderFactory.cpp
	source/ShaderItem.cpp
	source/Skybox.cpp
	source/SkyboxRenderer.cpp
	source/Transform.cpp
	source/Texture.cpp
	source/TextureFactory.cpp
	source/Camera.cpp
	source/Lights.cpp
	source/GlobalLights.cpp
	)

set(GRAPHICS_ALL "${GRAPHICS_INCLUDE}" "${GRAPHICS_SOURCE}")

CreateModule(engine graphics "${GRAPHICS_ALL}")

target_link_libraries(graphics GLFW Vulkan::Vulkan gli base)
