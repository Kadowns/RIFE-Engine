#include <ShaderFactory.h>

namespace Rife::Graphics {

	Shader * ShaderFactory::defaultShader(const std::string & vertShaderName, const std::string & fragShaderName) {
		return nullptr;
	}

	std::vector<char> ShaderFactory::loadShaderFile(const std::string& filename) {
		//abre o arquivo, começa a ler pelo final e em binario
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		//se n abriu o arquivo, pula fora
		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}

		//Cria o buffer com o tamanho do arquivo
		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		//volta para o começo do arquivo
		file.seekg(0);
		//passa os dados do arquio pro buffer
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}
	VkDescriptorSetLayoutBinding ShaderFactory::createDescriptorSetLayoutBinding(
		uint32_t binding,
		VkDescriptorType type,
		uint32_t descriptorCount,
		VkShaderStageFlags flags,
		const VkSampler * pImmutableSamplers
	) {

		VkDescriptorSetLayoutBinding descriptorBinding = {};
		descriptorBinding.binding = binding;
		descriptorBinding.descriptorType = type;
		descriptorBinding.descriptorCount = descriptorCount;
		descriptorBinding.stageFlags = flags;
		descriptorBinding.pImmutableSamplers = pImmutableSamplers;

		return descriptorBinding;
	}

	VkShaderModule ShaderFactory::createShaderModule(const std::vector<char>& code) {

		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		//Cast de char* para uint32_t* 
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(*VK_WRAPPER->getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shader module!");
		}
		return shaderModule;
	}

	VkPipelineShaderStageCreateInfo ShaderFactory::createShaderStage(
		VkShaderStageFlagBits stage,
		VkShaderModule module,
		const char * pName
	) {
		
		VkPipelineShaderStageCreateInfo shaderStageInfo = {};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = stage;
		shaderStageInfo.module = module;
		shaderStageInfo.pName = pName;
		return shaderStageInfo;

	}

	VkPipelineVertexInputStateCreateInfo ShaderFactory::createVertexInputStateInfo() {

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};

		auto bindingDescription = Vertex::getBindingDescription();
		auto attributeDescriptions = Vertex::getAttributeDescriptions();

		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;

		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		return vertexInputInfo;
	}

	VkPipelineInputAssemblyStateCreateInfo ShaderFactory::createInputAssemblyInfo(
		VkPipelineInputAssemblyStateCreateFlags flags,
		VkPrimitiveTopology topology,
		VkBool32 primitiveRestartEnable,
		const void * pNext
	) {
		
		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = topology;
		inputAssembly.primitiveRestartEnable = primitiveRestartEnable;
		inputAssembly.pNext = pNext;
		return inputAssembly;
	}
}


