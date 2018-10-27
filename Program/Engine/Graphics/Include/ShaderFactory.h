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

		static VkShaderModule createShaderModule(const std::vector<char>& code);

		static VkPipelineShaderStageCreateInfo createShaderStage(
			VkShaderStageFlagBits stage,
			VkShaderModule module,
			const char* pName
		);

		static VkPipelineVertexInputStateCreateInfo createVertexInputStateInfo();
		
		static VkPipelineInputAssemblyStateCreateInfo createInputAssemblyInfo(
			VkPipelineInputAssemblyStateCreateFlags flags,
			VkPrimitiveTopology topology,
			VkBool32 primitiveRestartEnable,
			const void* pNext
		);
		


	};
}