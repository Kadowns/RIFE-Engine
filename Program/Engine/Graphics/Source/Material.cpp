#include <Material.h>

namespace Rife::Graphics {


    Material::Material(VkPipelineLayoutCreateInfo& pipelineLayoutCreateInfo, VkGraphicsPipelineCreateInfo& pipelineCreateInfo) {
        
        if (vkCreatePipelineLayout(*VK_WRAPPER->getDevice(), &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }

        pipelineCreateInfo.layout = m_pipelineLayout;

        if (vkCreateGraphicsPipelines(*VK_WRAPPER->getDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &m_pipeline) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline!");
        }

    }

    Material::~Material() {
        vkDestroyPipeline(*VK_WRAPPER->getDevice(), m_pipeline, nullptr);
        vkDestroyPipelineLayout(*VK_WRAPPER->getDevice(), m_pipelineLayout, nullptr);
    }
}
