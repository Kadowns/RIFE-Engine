#include <MeshRenderer.h>

void MeshRenderer::draw() {

}

void MeshRenderer::recordCmdBuffer() {
	m_commandBuffers.resize(VK_WRAPPER->getSwapChainImagesCount());

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = *VK_WRAPPER->getCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
	allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();

	if (vkAllocateCommandBuffers(*VK_WRAPPER->getDevice(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
	for (int i = 0; i < m_commandBuffers.size(); i++) {

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		beginInfo.pInheritanceInfo = nullptr; // Optional

		if (vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkBuffer vertexBuffers[] = { m_vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(m_commandBuffers[i], 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(m_commandBuffers[i], m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);

		vkCmdBindDescriptorSets(
			m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
			*VK_WRAPPER->getPipelineLayout(), 0, 1, &(*VK_WRAPPER->getDescriptorSets())[i], 0, nullptr
		);

		vkCmdPushConstants(m_commandBuffers[i], *VK_WRAPPER->getPipelineLayout(),
			VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Entity::FinalTransform), (void *)&m_finalTransform);

		vkCmdDrawIndexed(m_commandBuffers[i], p_mesh->getIndices().size(), 1, 0, 0, 0);

		if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}

	VK_WRAPPER->bindCmdBuffer(&m_commandBuffers);
}

void MeshRenderer::setMVP(glm::mat4& vp) {
	glm::mat4 model(1);
	glm::translate(model, p_father->getTransform()->position);
	m_finalTransform.mvp = model * vp;
}

MeshRenderer::MeshRenderer(Mesh* mesh, Entity::Object *father) {
    p_mesh = mesh;
	p_father = father;

	VkDeviceSize bufferSize = sizeof(mesh->getVertices()[0]) * mesh->getVertices().size();
	VK_WRAPPER->createVertexBuffer(m_vertexBuffer, m_vertexBufferMemory, bufferSize, mesh->getVertices().data());

	bufferSize = sizeof(mesh->getIndices()[0]) * mesh->getIndices().size();
	VK_WRAPPER->createVertexBuffer(m_indexBuffer, m_indexBufferMemory, bufferSize, mesh->getIndices().data());
}
