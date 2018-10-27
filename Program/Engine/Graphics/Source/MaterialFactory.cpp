#include <MaterialFactory.h>

namespace Rife::Graphics {

    Material* MaterialFactory::defaultMaterial(const std::string& vertShaderName, const std::string& fragShaderName) {

		auto shader = ShaderFactory::defaultShader(vertShaderName, fragShaderName);

		MaterialProperties properties = {};
		properties.ambient = glm::vec4(0.5f, 0.0f, 0.5f, 0.0f);
		properties.diffuse = glm::vec4(0.5f, 0.0f, 0.5f, 0.0f);
		properties.specular = glm::vec4(1.0f, 1.0f, 1.0f, 64.0f);

		auto material = MaterialBuilder()
			.addDescriptorSetLayoutInfo(descriptorSetLayoutInfo)
			.setShaderStages(shaderStages, 2)
			.setVertexInputState(vertexInputInfo)
			.setInputAssemblyState(inputAssembly)
			.setViewportState(viewportState)
			.setRasterizationState(rasterizer)
			.setMultisampleState(multisampling)
			.setDepthStencilState(depthStencil)
			.setColorBlendState(colorBlending)
			.setMaterialProperties(properties)
            .createMaterial();

        vkDestroyShaderModule(*VK_WRAPPER->getDevice(), fragShaderModule, nullptr);
        vkDestroyShaderModule(*VK_WRAPPER->getDevice(), vertShaderModule, nullptr);
        return material;
    }

    std::vector<char> MaterialFactory::loadShaderFile(const std::string& filename) {
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

    VkShaderModule MaterialFactory::createShaderModule(const std::vector<char>& code) {

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
}


