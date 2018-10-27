#include <ShaderBuilder.h>

namespace Rife::Graphics {

	ShaderBuilder& ShaderBuilder::addDescriptorSetLayoutInfo(VkDescriptorSetLayoutCreateInfo& descriptorLayoutInfo) {
		m_descriptorSetLayoutInfos.push_back(descriptorLayoutInfo);
		return *this;
	}

	ShaderBuilder& ShaderBuilder::addPushConstantRange(VkPushConstantRange& pushConstantRange) {
		m_pushConstantRanges.push_back(pushConstantRange);
		return *this;
	}

	ShaderBuilder& ShaderBuilder::setShaderStages(VkPipelineShaderStageCreateInfo* shaderStages, const uint32_t& stageCount) {
		m_pipelineInfo.stageCount = stageCount;
		m_pipelineInfo.pStages = shaderStages;
		return *this;
	}

	ShaderBuilder& ShaderBuilder::setVertexInputState(VkPipelineVertexInputStateCreateInfo& vertexInputInfo) {
		m_pipelineInfo.pVertexInputState = &vertexInputInfo;
		return *this;
	}

	ShaderBuilder& ShaderBuilder::setInputAssemblyState(VkPipelineInputAssemblyStateCreateInfo& inputAssembly) {
		m_pipelineInfo.pInputAssemblyState = &inputAssembly;
		return *this;
	}

	ShaderBuilder& ShaderBuilder::setViewportState(VkPipelineViewportStateCreateInfo& viewportState) {
		m_pipelineInfo.pViewportState = &viewportState;
		return *this;
	}

	ShaderBuilder& ShaderBuilder::setRasterizationState(VkPipelineRasterizationStateCreateInfo& rasterizer) {
		m_pipelineInfo.pRasterizationState = &rasterizer;
		return *this;
	}

	ShaderBuilder& ShaderBuilder::setMultisampleState(VkPipelineMultisampleStateCreateInfo& multisampling) {
		m_pipelineInfo.pMultisampleState = &multisampling;
		return *this;
	}

	ShaderBuilder& ShaderBuilder::setDepthStencilState(VkPipelineDepthStencilStateCreateInfo& depthStencil) {
		m_pipelineInfo.pDepthStencilState = &depthStencil;
		return *this;
	}

	ShaderBuilder& ShaderBuilder::setColorBlendState(VkPipelineColorBlendStateCreateInfo& colorBlending) {
		m_pipelineInfo.pColorBlendState = &colorBlending;
		return *this;
	}

	Shader* ShaderBuilder::createShader() {
		return new Shader(m_pipelineInfo, m_descriptorSetLayoutInfos, m_pushConstantRanges);
	}
}