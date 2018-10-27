#include <MeshRenderer.h>
using namespace Rife::Graphics;

void MeshRenderer::draw() {

}

void MeshRenderer::createCommandBuffers() {
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

        vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *p_material->getShader()->getPipeline());

		VkBuffer vertexBuffers[] = { m_vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(m_commandBuffers[i], 0, 1, vertexBuffers, offsets);
        
		vkCmdBindIndexBuffer(m_commandBuffers[i], m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);

		vkCmdBindDescriptorSets(
			m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
			*p_material->getShader()->getPipelineLayout(), 0, 1, &m_descriptorSets[i], 0, nullptr
		);

		vkCmdDrawIndexed(m_commandBuffers[i], p_mesh->getIndices().size(), 1, 0, 0, 0);

		if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
	VK_WRAPPER->bindCommandBuffer(&m_commandBuffers);
}

void MeshRenderer::freeCommandBuffers() {

    vkFreeCommandBuffers(
        *VK_WRAPPER->getDevice(),
        *VK_WRAPPER->getCommandPool(),
        static_cast<uint32_t>(m_commandBuffers.size()),
        m_commandBuffers.data()
    );  
}


void MeshRenderer::updateTransformInformation(const glm::mat4& vp, const glm::vec3& cameraPos, const uint32_t& imageIndex) {

	//transform
	m_ubo.vp = vp;
	m_ubo.m = p_gameObject->getTransform()->getModelMatrix();
	m_ubo.cameraPos = glm::vec4(cameraPos, 0.0f);

	size_t offset = 0;
	size_t range = sizeof(m_ubo);
	void* data;

	vkMapMemory(*VK_WRAPPER->getDevice(), m_uniformBuffersMemory[imageIndex], offset, range, 0, &data);
	memcpy(data, &m_ubo, range);
	vkUnmapMemory(*VK_WRAPPER->getDevice(), m_uniformBuffersMemory[imageIndex]);
	//------------------------
	//light
	offset += range;
	range = sizeof(Light);
    data = nullptr;

    Light light = {};
    light.direction = glm::vec4(0.0f, -0.4, -1.0f, 0.0f);
	light.ambient = glm::vec4(0.1f);
	light.diffuse = glm::vec4(1.0f, 1.0f, 0.8f, 0.0f);
	light.specular = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    
    vkMapMemory(*VK_WRAPPER->getDevice(), m_uniformBuffersMemory[imageIndex], offset, range, 0, &data);
    memcpy(data, &light, range);
    vkUnmapMemory(*VK_WRAPPER->getDevice(), m_uniformBuffersMemory[imageIndex]);
	//-------------------
	//material
	offset += range;
	range = sizeof(MaterialProperties);
	data = nullptr;
	
	vkMapMemory(*VK_WRAPPER->getDevice(), m_uniformBuffersMemory[imageIndex], offset, range, 0, &data);
	memcpy(data, &(p_material->getProperties()), range);
	vkUnmapMemory(*VK_WRAPPER->getDevice(), m_uniformBuffersMemory[imageIndex]);
	//--------------------
}

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material) {
    p_mesh = mesh;
    p_material = material;

	VkDeviceSize bufferSize = sizeof(p_mesh->getVertices()[0]) * p_mesh->getVertices().size();
	VK_WRAPPER->createVertexBuffer(m_vertexBuffer, m_vertexBufferMemory, bufferSize, p_mesh->getVertices().data());

	bufferSize = sizeof(p_mesh->getIndices()[0]) * p_mesh->getIndices().size();
	VK_WRAPPER->createIndexBuffer(m_indexBuffer, m_indexBufferMemory, bufferSize, p_mesh->getIndices().data());


    bufferSize = sizeof(Ubo::Transform) + sizeof(Light) + sizeof(MaterialProperties);
	m_uniformBuffers.resize(VK_WRAPPER->getSwapChainImagesCount());
	m_uniformBuffersMemory.resize(VK_WRAPPER->getSwapChainImagesCount());
	for (int i = 0; i < m_uniformBuffers.size(); i++) {
		VK_WRAPPER->createUniformBuffer(m_uniformBuffers[i], m_uniformBuffersMemory[i], bufferSize);
	}	
	createDescriptorPool();
	createDescriptorSets();
	VK_WRAPPER->bindRenderer(this);
}


MeshRenderer::~MeshRenderer() {
	vkDestroyDescriptorPool(*VK_WRAPPER->getDevice(), m_descriptorPool, nullptr);

	vkDestroyBuffer(*VK_WRAPPER->getDevice(), m_vertexBuffer, nullptr);
	vkFreeMemory(*VK_WRAPPER->getDevice(), m_vertexBufferMemory, nullptr);

	vkDestroyBuffer(*VK_WRAPPER->getDevice(), m_indexBuffer, nullptr);
	vkFreeMemory(*VK_WRAPPER->getDevice(), m_indexBufferMemory, nullptr);

	for (int i = 0; i < m_uniformBuffers.size(); i++) {
		vkDestroyBuffer(*VK_WRAPPER->getDevice(), m_uniformBuffers[i], nullptr);
		vkFreeMemory(*VK_WRAPPER->getDevice(), m_uniformBuffersMemory[i], nullptr);
	}

    if (p_material != nullptr) {
        p_material->~Material();
    }
	
    freeCommandBuffers();
}

void MeshRenderer::createDescriptorPool() {
	std::array<VkDescriptorPoolSize, 3> poolSizes = {};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = VK_WRAPPER->getSwapChainImagesCount();
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[1].descriptorCount = VK_WRAPPER->getSwapChainImagesCount();
	poolSizes[2].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[2].descriptorCount = VK_WRAPPER->getSwapChainImagesCount();


	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(poolSizes[0].descriptorCount);

	if (vkCreateDescriptorPool(*VK_WRAPPER->getDevice(), &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void MeshRenderer::createDescriptorSets() {

	std::vector<VkDescriptorSetLayout> layouts(VK_WRAPPER->getSwapChainImagesCount(), *(p_material->getShader()->getDescriptorSetLayouts())->data());
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(VK_WRAPPER->getSwapChainImagesCount());
	allocInfo.pSetLayouts = layouts.data();

	m_descriptorSets.resize(VK_WRAPPER->getSwapChainImagesCount());
	if (vkAllocateDescriptorSets(*VK_WRAPPER->getDevice(), &allocInfo, m_descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}
	std::array<VkDescriptorBufferInfo, 3> bufferInfos = {};
	temp_offsets.resize(bufferInfos.size());
	for (size_t i = 0; i < m_descriptorSets.size(); i++) {

		
		bufferInfos[0].buffer = m_uniformBuffers[i];
		bufferInfos[0].offset = 0;
		bufferInfos[0].range = sizeof(Ubo::Transform);
        auto properties = VK_WRAPPER->getPhysicalDeviceProperties();
        

        bufferInfos[1].buffer = m_uniformBuffers[i];
		bufferInfos[1].offset = (bufferInfos[0].range + bufferInfos[0].offset);
        bufferInfos[1].range = sizeof(Light);

		bufferInfos[2].buffer = m_uniformBuffers[i];
		bufferInfos[2].offset = (bufferInfos[1].range + bufferInfos[1].offset);
		bufferInfos[2].range = sizeof(MaterialProperties);


		std::array<VkWriteDescriptorSet, 3> descriptorWrite = {};
		for (size_t j = 0; j < descriptorWrite.size(); j++) {
			descriptorWrite[j].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite[j].dstSet = m_descriptorSets[i];
			descriptorWrite[j].dstBinding = j;
			descriptorWrite[j].dstArrayElement = 0;
			descriptorWrite[j].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite[j].descriptorCount = 1;
			descriptorWrite[j].pBufferInfo = &bufferInfos[j];
		}

		vkUpdateDescriptorSets(*VK_WRAPPER->getDevice(), static_cast<uint32_t>(descriptorWrite.size()), descriptorWrite.data(), 0, nullptr);
	}
	for (int i = 0; i < bufferInfos.size(); i++) {
		temp_offsets[i] = bufferInfos[i].offset;
	}
}
