#include <MeshRenderer.h>

void MeshRenderer::draw() {

}

void MeshRenderer::recordCmdBuffer() {

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = *VK_WRAPPER->getCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
	allocInfo.commandBufferCount = 1;

	if (vkAllocateCommandBuffers(*VK_WRAPPER->getDevice(), &allocInfo, &m_vkCommandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

MeshRenderer::MeshRenderer(Mesh * mesh) {
    p_mesh = mesh;
}
