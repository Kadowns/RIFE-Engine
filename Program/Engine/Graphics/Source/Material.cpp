#include <Material.h>

namespace Rife::Graphics {


    Material::Material(MaterialProperties& properties, std::vector<VkDescriptorSetLayoutCreateInfo>& descriptorSetLayoutInfos,
        std::vector<VkPushConstantRange>& pushConstantRangeInfos, VkGraphicsPipelineCreateInfo& pipelineCreateInfo) {

		m_properties = properties;

        
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
