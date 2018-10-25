#include <MaterialBuilder.h>

namespace Rife::Graphics {
    MaterialBuilder& MaterialBuilder::addDescriptorSetLayoutInfo(VkDescriptorSetLayoutCreateInfo& descriptorLayoutInfo) {
        m_descriptorSetLayoutInfos.push_back(descriptorLayoutInfo);
        return *this;
    }
    MaterialBuilder& MaterialBuilder::addPushConstantRange(VkPushConstantRange& pushConstantRange) {
        m_pushConstantRanges.push_back(pushConstantRange);
        return *this;
    }
    MaterialBuilder& MaterialBuilder::setShaderStages(VkPipelineShaderStageCreateInfo* shaderStages, const uint32_t& stageCount) {
        m_pipelineInfo.stageCount = stageCount;
        m_pipelineInfo.pStages = shaderStages;
        return *this;
    }
    MaterialBuilder& MaterialBuilder::setVertexInputState(VkPipelineVertexInputStateCreateInfo& vertexInputInfo) {
        m_pipelineInfo.pVertexInputState = &vertexInputInfo;
        return *this;
    }
    MaterialBuilder& MaterialBuilder::setInputAssemblyState(VkPipelineInputAssemblyStateCreateInfo& inputAssembly) {
        m_pipelineInfo.pInputAssemblyState = &inputAssembly;
        return *this;
    }
    MaterialBuilder& MaterialBuilder::setViewportState(VkPipelineViewportStateCreateInfo& viewportState) {
        m_pipelineInfo.pViewportState = &viewportState;
        return *this;
    }
    MaterialBuilder& MaterialBuilder::setRasterizationState(VkPipelineRasterizationStateCreateInfo& rasterizer) {
        m_pipelineInfo.pRasterizationState = &rasterizer;
        return *this;
    }
    MaterialBuilder& MaterialBuilder::setMultisampleState(VkPipelineMultisampleStateCreateInfo& multisampling) {
        m_pipelineInfo.pMultisampleState = &multisampling;
        return *this;
    }
    MaterialBuilder& MaterialBuilder::setDepthStencilState(VkPipelineDepthStencilStateCreateInfo& depthStencil) {
        m_pipelineInfo.pDepthStencilState = &depthStencil;
        return *this;
    }
    MaterialBuilder& MaterialBuilder::setColorBlendState(VkPipelineColorBlendStateCreateInfo& colorBlending) {
        m_pipelineInfo.pColorBlendState = &colorBlending;
        return *this;
    }
	MaterialBuilder& MaterialBuilder::setMaterialProperties(MaterialProperties& properties) {
		m_properties = properties;
		return *this;
	}
    Material* MaterialBuilder::createMaterial() {
        return new Material(m_properties, m_descriptorSetLayoutInfos, m_pushConstantRanges, m_pipelineInfo);
    }
}


