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

        VkCommandBufferInheritanceInfo inheritanceInfo = {};
        inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
        inheritanceInfo.pNext = nullptr;
        inheritanceInfo.renderPass = *VK_WRAPPER->getRenderPass();
        inheritanceInfo.framebuffer = (*VK_WRAPPER->getFramebuffers())[i];
        inheritanceInfo.subpass = 0;


		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
		beginInfo.pInheritanceInfo = &inheritanceInfo;

		if (vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

        vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *VK_WRAPPER->getGraphicsPipeline());

		VkBuffer vertexBuffers[] = { m_vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(m_commandBuffers[i], 0, 1, vertexBuffers, offsets);

        
		vkCmdBindIndexBuffer(m_commandBuffers[i], m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);

        glm::mat4 model(1);
        model = glm::translate(model, p_father->getTransform()->position);
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        m_ubo.mvp = CAMERA->getProjection() * CAMERA->getView() * model;

        vkCmdPushConstants(m_commandBuffers[i], *VK_WRAPPER->getPipelineLayout(),
            VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Graphics::UniformBufferObject), (void*)&m_ubo);

		vkCmdBindDescriptorSets(
			m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
			*VK_WRAPPER->getPipelineLayout(), 0, 1, &m_descriptorSets[i], 0, nullptr
		);

		vkCmdDrawIndexed(m_commandBuffers[i], p_mesh->getIndices().size(), 1, 0, 0, 0);

		if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
	VK_WRAPPER->bindCmdBuffer(&m_commandBuffers);
}

void MeshRenderer::updateTransformInformation(glm::mat4& vp, uint32_t imageIndex) {
	glm::mat4 model(1);
	model = glm::translate(model, p_father->getTransform()->position);
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	m_ubo.mvp = CAMERA->getProjection() * CAMERA->getView() * model;

	/*void* data;
	vkMapMemory(*VK_WRAPPER->getDevice(), m_uniformBuffersMemory[imageIndex], 0, sizeof(m_ubo), 0, &data);
	memcpy(data, &m_ubo, sizeof(m_ubo));
	vkUnmapMemory(*VK_WRAPPER->getDevice(), m_uniformBuffersMemory[imageIndex]);*/
}

MeshRenderer::MeshRenderer(Mesh* mesh, Entity::Object *father) {
    p_mesh = mesh;
	p_father = father;

	VkDeviceSize bufferSize = sizeof(mesh->getVertices()[0]) * mesh->getVertices().size();
	VK_WRAPPER->createVertexBuffer(m_vertexBuffer, m_vertexBufferMemory, bufferSize, mesh->getVertices().data());

	bufferSize = sizeof(mesh->getIndices()[0]) * mesh->getIndices().size();
	VK_WRAPPER->createIndexBuffer(m_indexBuffer, m_indexBufferMemory, bufferSize, mesh->getIndices().data());


	bufferSize = sizeof(Graphics::UniformBufferObject);
	m_uniformBuffers.resize(VK_WRAPPER->getSwapChainImagesCount());
	m_uniformBuffersMemory.resize(VK_WRAPPER->getSwapChainImagesCount());
	for (int i = 0; i < m_uniformBuffers.size(); i++) {
		VK_WRAPPER->createUniformBuffer(m_uniformBuffers[i], m_uniformBuffersMemory[i], bufferSize);
	}	
	createDescriptorPool();
	createDescriptorSets();
}

MeshRenderer::~MeshRenderer() {
	vkDestroyDescriptorPool(*VK_WRAPPER->getDevice(), m_descriptorPool, nullptr);

}

void MeshRenderer::createDescriptorPool() {
	VkDescriptorPoolSize poolSize = {};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = VK_WRAPPER->getSwapChainImagesCount();

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = static_cast<uint32_t>(poolSize.descriptorCount);

	if (vkCreateDescriptorPool(*VK_WRAPPER->getDevice(), &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void MeshRenderer::createDescriptorSets() {

	std::vector<VkDescriptorSetLayout> layouts(VK_WRAPPER->getSwapChainImagesCount(), *VK_WRAPPER->getDescriptorSetLayout());
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(VK_WRAPPER->getSwapChainImagesCount());
	allocInfo.pSetLayouts = layouts.data();

	m_descriptorSets.resize(VK_WRAPPER->getSwapChainImagesCount());
	if (vkAllocateDescriptorSets(*VK_WRAPPER->getDevice(), &allocInfo, m_descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < m_descriptorSets.size(); i++) {

		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = m_uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(Graphics::UniformBufferObject);

		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = m_descriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;

		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;

		descriptorWrite.pBufferInfo = &bufferInfo;
		descriptorWrite.pImageInfo = nullptr; // Optional
		descriptorWrite.pTexelBufferView = nullptr; // Optional

		vkUpdateDescriptorSets(*VK_WRAPPER->getDevice(), 1, &descriptorWrite, 0, nullptr);
	}


}
