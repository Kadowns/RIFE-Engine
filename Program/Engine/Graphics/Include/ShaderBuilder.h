#pragma once

#include <Shader.h>

namespace Rife::Graphics {

	class ShaderBuilder {
	public:

		ShaderBuilder& addDescriptorSetLayoutInfo(VkDescriptorSetLayoutCreateInfo& descriptorLayoutInfo);
		ShaderBuilder& addPushConstantRange(VkPushConstantRange& pushConstantRange);
        ShaderBuilder& addUniformBufferObjectInfo(UniformBufferObjectInfo& uboInfo);
        ShaderBuilder& setLayoutBindings(std::vector<VkDescriptorSetLayoutBinding>& layoutBindings);
        ShaderBuilder& setShaderNames(std::vector<std::string>& m_names);
        ShaderBuilder& setRasterizer(VkPipelineRasterizationStateCreateInfo& rasterizer);
		ShaderBuilder& setVertexInputBinding(VkVertexInputBindingDescription& vertexInputBinding);
        ShaderBuilder& setVertexAttribute(std::vector<VkVertexInputAttributeDescription>& vertexAttribute);	
		ShaderBuilder& setDepthStencil(VkPipelineDepthStencilStateCreateInfo& depthStencil);
		ShaderBuilder& setColorBlend(VkPipelineColorBlendAttachmentState& colorBlending);
		Shader* createShader();

	private:

		std::vector<VkDescriptorSetLayoutCreateInfo> m_descriptorSetLayoutInfos;
		std::vector<VkPushConstantRange> m_pushConstantRanges;
        std::vector<UniformBufferObjectInfo> m_uboInfo;
        std::vector<VkDescriptorSetLayoutBinding> m_layoutBindings;
        std::vector<std::string> m_names;

        VkVertexInputBindingDescription m_vertexBinding;
        std::vector<VkVertexInputAttributeDescription> m_vertexAttributes;
        VkPipelineRasterizationStateCreateInfo m_rasterizer;
        VkPipelineDepthStencilStateCreateInfo m_depthStencil;
        VkPipelineColorBlendAttachmentState m_colorBlend;

	};

}