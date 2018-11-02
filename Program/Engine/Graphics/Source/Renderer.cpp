#include <Renderer.h>
#include <VulkanTools.h>

namespace Rife::Graphics {

	void Renderer::createUniformBuffer(VkBuffer& buffer, VkDeviceMemory& memory, VkDeviceSize bufferSize) {
		VulkanTools::createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			buffer,
			memory
		);
	}

}