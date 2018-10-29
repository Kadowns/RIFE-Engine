#pragma once

#include <ShaderBuilder.h>

namespace Rife::Graphics {

	class ShaderFactory {

	public:

		static Shader* defaultShader(const std::string& vertShaderName, const std::string& fragShaderName);

	private:

		static std::vector<char> loadShaderFile(const std::string& filename);

		static VkDescriptorSetLayoutBinding createDescriptorSetLayoutBinding(
			uint32_t binding,
			VkDescriptorType type,
			uint32_t descriptorCount,
			VkShaderStageFlags flags,
			const VkSampler* pImmutableSamplers
		);

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