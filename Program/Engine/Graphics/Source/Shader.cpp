#include "Shader.h"

gph::ShaderModule* gph::ShaderModule::bindDevice(VkDevice* vkdevice) {
   
    if (vkdevice == nullptr) {
        this->unbindDevice();
        return this;
    }
    p_vkDevice = vkdevice;
    return this;
}

gph::ShaderModule* gph::ShaderModule::bindShader(const std::string& filename, VkShaderStageFlagBits shaderType) {
    if (p_vkDevice == nullptr) {
        throw std::runtime_error("No Device was bound!");
    }

    //OS BINÁRIOS DO SHADER PRECISAM ESTAR NA MESMA PASTA DO EXECUTAVEL, FICA LÁ EM CMAKE/BUILDS/BLABLABLA
    auto shaderCode = ShaderModule::loadShaderFile(filename);

    VkShaderModule shaderModule = createShaderModule(shaderCode);

    //Prepara o shader
    VkPipelineShaderStageCreateInfo shaderStageInfo = {};
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = shaderType;
    shaderStageInfo.module = shaderModule;
    shaderStageInfo.pName = "main";
    //--------
    m_shadersInfo.push_back(shaderStageInfo);

    return this;
    
}

gph::ShaderModule* gph::ShaderModule::unbindDevice() {
    for (int i = 0; i < m_shadersInfo.size(); i++) {
        vkDestroyShaderModule(*p_vkDevice, m_shadersInfo[i].module, nullptr);
    }
    m_shadersInfo.clear();
    p_vkDevice = nullptr;
    return this;
}

std::vector<VkPipelineShaderStageCreateInfo>* gph::ShaderModule::getShadersInfo() {
    if (p_vkDevice == nullptr) {
        throw std::runtime_error("Device was not bound!");
    }

    return &m_shadersInfo;
}


VkShaderModule gph::ShaderModule::createShaderModule(const std::vector<char>& code) {

    if (p_vkDevice == nullptr) {
        throw std::runtime_error("Device was not bound!");
    }

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    //Cast de char* para uint32_t* 
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(*(p_vkDevice), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }
    return shaderModule;
}


std::vector<char> gph::ShaderModule::loadShaderFile(const std::string & filename) {

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
