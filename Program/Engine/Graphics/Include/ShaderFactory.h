#pragma once

#include <ShaderBuilder.h>

namespace Rife::Graphics {

	class ShaderFactory {

	public:

		static Shader* surfaceShader(const std::string& vertShaderName, const std::string& fragShaderName);
        static Shader* defaultShader();
        static Shader* skyboxShader();
        static Shader* terrainShader();

	private:

		static std::vector<char> loadShaderFile(const std::string& filename);

		static VkDescriptorSetLayoutBinding createDescriptorSetLayoutBinding(
			uint32_t binding,
			VkDescriptorType type,
			uint32_t descriptorCount,
			VkShaderStageFlags flags,
			const VkSampler* pImmutableSamplers
		);

        static VkPushConstantRange createPushConstantRange(VkShaderStageFlags stage, uint32_t range, uint32_t offset);

        static UniformBufferObjectInfo createUboInfo(
			VkDeviceSize size, SHADER_ITEM_TYPE type
        );

		static VkShaderModule createShaderModule(const std::vector<char>& code);

		static VkPipelineShaderStageCreateInfo createShaderStage(
			VkShaderStageFlagBits stage,
			VkShaderModule module,
			const char* pName
		);

		static VkPipelineVertexInputStateCreateInfo createVertexInputInfo(
            VkVertexInputBindingDescription&,
            const std::vector<VkVertexInputAttributeDescription>&
        );
		
		static VkPipelineInputAssemblyStateCreateInfo createInputAssemblyInfo(
			VkPrimitiveTopology topology,
			VkBool32 primitiveRestartEnable,
			const void* pNext
		);

		static VkPipelineViewportStateCreateInfo createViewportInfo(VkViewport&, VkRect2D&);
		
		static VkPipelineRasterizationStateCreateInfo createRasterizerInfo(
			const VkBool32& depthClamp,
			VkPolygonMode fillMode,
			VkCullModeFlags cullMode,
			VkFrontFace frontFace
		);

		static VkPipelineMultisampleStateCreateInfo createMultisampleInfo();

		static VkPipelineColorBlendStateCreateInfo createColorBlendInfo(VkPipelineColorBlendAttachmentState&);

		static VkPipelineDepthStencilStateCreateInfo createDepthStencilInfo(
            const VkBool32& testEnabled, const VkBool32& writeEnabled, const VkCompareOp& compareOp
        );
	};
}