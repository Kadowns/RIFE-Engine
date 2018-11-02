#include <VulkanData.h>
#include <VulkanBase.h>

#include <MeshRenderer.h>
#include <MaterialFactory.h>
#include <Transform.h>
#include <Camera.h>
#include <Lights.h>

using namespace Rife::Graphics;

MeshRenderer::MeshRenderer(Mesh* mesh, const MaterialInstance& material) {
    p_mesh = mesh;
    m_material = material;
}

MeshRenderer::~MeshRenderer() {
    vkDestroyDescriptorPool(VK_DATA->getDevice(), m_descriptorPool, nullptr);

    for (int i = 0; i < m_uniformBuffers.size(); i++) {
        vkDestroyBuffer(VK_DATA->getDevice(), m_uniformBuffers[i], nullptr);
        vkFreeMemory(VK_DATA->getDevice(), m_uniformBuffersMemory[i], nullptr);
    }

    freeCommandBuffers();
}

void Rife::Graphics::MeshRenderer::setup() {

    VkDeviceSize bufferSize = m_material.getShader()->getUboOffset(m_material.getShader()->getUboSize());
    m_uniformBuffers.resize(VK_DATA->getSwapchainImages().size());
    m_uniformBuffersMemory.resize(VK_DATA->getSwapchainImages().size());
    for (int i = 0; i < m_uniformBuffers.size(); i++) {
        createUniformBuffer(m_uniformBuffers[i], m_uniformBuffersMemory[i], bufferSize);
    }
    createDescriptorPool();
    createDescriptorSets();
    VK_BASE->bindRenderer(this);

}

void MeshRenderer::createCommandBuffers() {
	m_commandBuffers.resize(VK_DATA->getSwapchainImages().size());

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = VK_DATA->getCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
	allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();

	if (vkAllocateCommandBuffers(VK_DATA->getDevice(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}

	for (int i = 0; i < m_commandBuffers.size(); i++) {

        VkCommandBufferInheritanceInfo inheritanceInfo = {};
        inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
        inheritanceInfo.pNext = nullptr;
        inheritanceInfo.renderPass = VK_DATA->getRenderPass();
        inheritanceInfo.framebuffer = (VK_DATA->getFramebuffers())[i];
        inheritanceInfo.subpass = 0;


		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
		beginInfo.pInheritanceInfo = &inheritanceInfo;

		if (vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

        vkCmdPushConstants(
            m_commandBuffers[i],
            *m_material.getShader()->getPipelineLayout(),
            VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(Ubo::uMaterialProperties),
            (void*)&m_material.getProperties()
        );

        vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *m_material.getShader()->getPipeline());

		VkBuffer vertexBuffers[] = { p_mesh->getVertexBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(m_commandBuffers[i], 0, 1, vertexBuffers, offsets);
        
		vkCmdBindIndexBuffer(m_commandBuffers[i], p_mesh->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);

		vkCmdBindDescriptorSets(
			m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
			*m_material.getShader()->getPipelineLayout(), 0, 1, &m_descriptorSets[i], 0, nullptr
		);

		vkCmdDrawIndexed(m_commandBuffers[i], p_mesh->getIndices().size(), 1, 0, 0, 0);

		if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
	VK_BASE->bindCommandBuffer(&m_commandBuffers);
}

void MeshRenderer::freeCommandBuffers() {

    vkFreeCommandBuffers(
        VK_DATA->getDevice(),
        VK_DATA->getCommandPool(),
        static_cast<uint32_t>(m_commandBuffers.size()),
        m_commandBuffers.data()
    );  
}


void MeshRenderer::submitUniformBuffersInfo(const uint32_t& imageIndex) {
  
    m_material.getShader()
        ->bindUniformBufferMemory(&m_uniformBuffersMemory[imageIndex])
        .setItem(*p_gameObject->getComponent<Transform>())
        .setItem(*CAMERA)
        .setItem(*GLOBAL_LIGHTS);
}

void MeshRenderer::createDescriptorPool() {
    auto layoutBindings = m_material.getShader()->getLayoutBindings();

	std::vector<VkDescriptorPoolSize> poolSizes = {};
    poolSizes.resize(layoutBindings.size());
    for (size_t i = 0; i < layoutBindings.size(); i++) {
        poolSizes[i].type = layoutBindings[i].descriptorType;
        poolSizes[i].descriptorCount = VK_DATA->getSwapchainImages().size();
    }

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(poolSizes[0].descriptorCount);

	if (vkCreateDescriptorPool(VK_DATA->getDevice(), &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void MeshRenderer::createDescriptorSets() {

	std::vector<VkDescriptorSetLayout> layouts(VK_DATA->getSwapchainImages().size(), *(m_material.getShader()->getDescriptorSetLayouts()).data());
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(VK_DATA->getSwapchainImages().size());
	allocInfo.pSetLayouts = layouts.data();

	m_descriptorSets.resize(VK_DATA->getSwapchainImages().size());
	if (vkAllocateDescriptorSets(VK_DATA->getDevice(), &allocInfo, m_descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

    Shader* shader = m_material.getShader();

	std::vector<VkDescriptorBufferInfo> bufferInfos = {};
    std::vector<VkDescriptorImageInfo> imageInfos = {};
    bufferInfos.resize(shader->getUboSize());
    imageInfos.resize(m_material.getTexturesSize());
	for (size_t i = 0; i < m_descriptorSets.size(); i++) {
	
        for (size_t j = 0; j < bufferInfos.size(); j++) {
            bufferInfos[j].buffer = m_uniformBuffers[i];
            bufferInfos[j].offset = shader->getUboOffset(j);
            bufferInfos[j].range = shader->getUboInfo(j).range;
        }
        for (size_t j = 0; j < imageInfos.size(); j++) {
            Texture* tex = m_material.getTextures()[j];

            imageInfos[j].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfos[j].imageView = tex->getImageView();
            imageInfos[j].sampler = tex->getSampler();
        }
        
		std::vector<VkWriteDescriptorSet> descriptorWrite = {};
        descriptorWrite.resize(bufferInfos.size() + imageInfos.size());
		for (size_t j = 0; j < bufferInfos.size(); j++) {
			descriptorWrite[j].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite[j].dstSet = m_descriptorSets[i];
			descriptorWrite[j].dstBinding = j;
			descriptorWrite[j].dstArrayElement = 0;
			descriptorWrite[j].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite[j].descriptorCount = 1;
			descriptorWrite[j].pBufferInfo = &bufferInfos[j];
		}
        for (size_t j = bufferInfos.size(); j < descriptorWrite.size(); j++) {
            descriptorWrite[j].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite[j].dstSet = m_descriptorSets[i];
            descriptorWrite[j].dstBinding = j;
            descriptorWrite[j].dstArrayElement = 0;
            descriptorWrite[j].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrite[j].descriptorCount = 1;
            descriptorWrite[j].pImageInfo = &imageInfos[j - bufferInfos.size()];
        }
        

		vkUpdateDescriptorSets(VK_DATA->getDevice(), static_cast<uint32_t>(descriptorWrite.size()), descriptorWrite.data(), 0, nullptr);
	}
}
