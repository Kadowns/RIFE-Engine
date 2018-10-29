#pragma once

#include <Shader.h>

namespace Rife::Graphics {

	class ShaderBuilder {
	public:

		ShaderBuilder& addDescriptorSetLayoutInfo(VkDescriptorSetLayoutCreateInfo& descriptorLayoutInfo);
		ShaderBuilder& addPushConstantRange(VkPushConstantRange& pushConstantRange);
        ShaderBuilder& addUniformBufferObjectInfo(UniformBufferObjectInfo& uboInfo);
		ShaderBuilder& setShaderStages(VkPipelineShaderStageCreateInfo* shaderStages, const uint32_t& stageCount);
		ShaderBuilder& setVertexInputState(VkPipelineVertexInputStateCreateInfo& vertexInputInfo);
		ShaderBuilder& setInputAssemblyState(VkPipelineInputAssemblyStateCreateInfo& inputAssembly);
		ShaderBuilder& setViewportState(VkPipelineViewportStateCreateInfo& viewportState);
		ShaderBuilder& setRasterizationState(VkPipelineRasterizationStateCreateInfo& rasterizer);
		ShaderBuilder& setMultisampleState(VkPipelineMultisampleStateCreateInfo& multisampling);
		ShaderBuilder& setDepthStencilState(VkPipelineDepthStencilStateCreateInfo& depthStencil);
		ShaderBuilder& setColorBlendState(VkPipelineColorBlendStateCreateInfo& colorBlending);
		Shader* createShader();

	private:

		std::vector<VkDescriptorSetLayoutCreateInfo> m_descriptorSetLayoutInfos;
		std::vector<VkPushConstantRange> m_pushConstantRanges;
        std::vector<UniformBufferObjectInfo> m_uboInfo;
		VkGraphicsPipelineCreateInfo m_pipelineInfo;

	};

}