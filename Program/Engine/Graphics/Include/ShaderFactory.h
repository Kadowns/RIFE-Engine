#pragma once

#include <ShaderBuilder.h>

namespace Rife::Graphics {

	class ShaderFactory {

	public:

		static Shader* defaultShader(const std::string_view& vertShaderName, const std::string_view& fragShaderName);

	private:

		static std::vector<char> loadShaderFile(const std::string_view& filename);

		static VkDescriptorSetLayoutBinding createDescriptorSetLayoutBinding(
			uint32_t binding,
			VkDescriptorType type,
			uint32_t descriptorCount,
			VkShaderStageFlags flags,
			const VkSampler* pImmutableSamplers
		);

        static VkPushConstantRange createPushConstantRange(VkShaderStageFlags stage, uint32_t range, uint32_t offset);

        static UniformBufferObjectInfo createUboInfo(
            VkDeviceSize range
        );

		static VkShaderModule createShaderModule(const std::vector<char>& code);

		static VkPipelineShaderStageCreateInfo createShaderStage(
			VkShaderStageFlagBits stage,
			VkShaderModule module,
			const char* pName
		);

		static VkPipelineVertexInputStateCreateInfo createVertexInputInfo(
            VkVertexInputBindingDescription&,
            std::array<VkVertexInputAttributeDescription, 3>
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

		static VkPipelineDepthStencilStateCreateInfo createDepthStencilInfo();
	};
}