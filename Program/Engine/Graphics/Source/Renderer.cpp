#include <Renderer.h>

#include <VulkanTools.h>
#include <VulkanData.h>
#include <VulkanBase.h>

#include <Shader.h>
#include <Texture.h>


namespace Rife::Graphics {

    Renderer::~Renderer() {

        vkDestroyDescriptorPool(VK_DATA->getDevice(), m_descriptorPool, nullptr);

        freeCommandBuffers();
    }

    void Renderer::setup() {

        createDescriptorPool();
        createDescriptorSets();
        VK_BASE->bindRenderer(this);

    }

    void Renderer::freeCommandBuffers() {
        vkFreeCommandBuffers(
            VK_DATA->getDevice(),
            VK_DATA->getCommandPool(),
            static_cast<uint32_t>(m_commandBuffers.size()),
            m_commandBuffers.data()
        );
    }

    void Renderer::createDescriptorPool() {
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


    void Renderer::createDescriptorSets() {

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
}