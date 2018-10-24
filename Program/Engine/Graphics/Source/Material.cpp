#include <Material.h>

namespace Rife::Graphics {


    Material::Material(std::vector<VkDescriptorSetLayoutCreateInfo>& descriptorSetLayoutInfos,
        std::vector<VkPushConstantRange>& pushConstantRangeInfos, VkGraphicsPipelineCreateInfo& pipelineCreateInfo) {

        m_descriptorSetLayouts.resize(descriptorSetLayoutInfos.size());
        for (int i = 0; i < m_descriptorSetLayouts.size(); i++) {
            if (vkCreateDescriptorSetLayout( *VK_WRAPPER->getDevice(), &descriptorSetLayoutInfos[i], nullptr, &m_descriptorSetLayouts[i])
                != VK_SUCCESS) {
                throw std::runtime_error("failed to create descriptor set layout!");
            }
        }      

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.pNext = nullptr;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(m_descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = m_descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(pushConstantRangeInfos.size());
        pipelineLayoutInfo.pPushConstantRanges = pushConstantRangeInfos.data();
        
        if (vkCreatePipelineLayout(*VK_WRAPPER->getDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }

        pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipelineCreateInfo.basePipelineIndex = -1; // Optional
        pipelineCreateInfo.renderPass = *VK_WRAPPER->getRenderPass();
        pipelineCreateInfo.subpass = 0;
        pipelineCreateInfo.layout = m_pipelineLayout;
		

        if (vkCreateGraphicsPipelines(*VK_WRAPPER->getDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &m_pipeline) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline!");
        }
        m_clear = false;
    }

    Material::~Material() {
        clearPipeline();
        for (int i = 0; i < m_descriptorSetLayouts.size(); i++) {
            vkDestroyDescriptorSetLayout(*VK_WRAPPER->getDevice(), m_descriptorSetLayouts[i], nullptr);
        }       
    }

    void Material::clearPipeline() {
        if (m_clear)
            return;

        vkDestroyPipeline(*VK_WRAPPER->getDevice(), m_pipeline, nullptr);
        vkDestroyPipelineLayout(*VK_WRAPPER->getDevice(), m_pipelineLayout, nullptr);
        m_clear = true;
    }
}
