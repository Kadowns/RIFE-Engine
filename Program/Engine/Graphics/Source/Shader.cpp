#include "Shader.h"

gph::Shader gph::Shader::bindDevice(VkDevice* vkdevice) {
    if (m_this == nullptr) {
        if (vkdevice == nullptr) {
            return *m_this;
        }
        m_this = new Shader();
    }
    
    if (vkdevice == nullptr) {
        Shader::unbindDevice();
        return *m_this;
    }
    m_this->p_vkDevice = vkdevice;
    return *m_this;
}

gph::Shader gph::Shader::bindShader(const std::string& filename, VkShaderStageFlagBits shaderType) {
    if (m_this == nullptr || m_this->p_vkDevice == nullptr) {
        throw std::runtime_error("No Device was bound!");
    }

    //OS BINÁRIOS DO SHADER PRECISAM ESTAR NA MESMA PASTA DO EXECUTAVEL, FICA LÁ EM CMAKE/BUILDS/BLABLABLA
    auto shaderCode = Shader::loadShaderFile(filename);

    VkShaderModule shaderModule = createShaderModule(shaderCode);

    //Prepara o shader
    VkPipelineShaderStageCreateInfo shaderStageInfo = {};
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = shaderType;
    shaderStageInfo.module = shaderModule;
    shaderStageInfo.pName = "main";
    //--------
    m_this->m_shadersInfo.push_back(shaderStageInfo);

    return *m_this;
}

void gph::Shader::unbindDevice() {
    for (int i = 0; i < m_this->m_shadersInfo.size(); i++) {
        vkDestroyShaderModule(*m_this->p_vkDevice, m_this->m_shadersInfo[i].module, nullptr);
    }
    m_this->m_shadersInfo.clear();
    delete m_this;
    m_this = nullptr;
}

std::vector<VkPipelineShaderStageCreateInfo>* gph::Shader::getShadersInfo() {
    if (m_this == nullptr) {
        throw std::runtime_error("Device was not bound!");
    }

    return &m_this->m_shadersInfo;
}


VkShaderModule gph::Shader::createShaderModule(const std::vector<char>& code) {

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    //Cast de char* para uint32_t* 
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(*(m_this->p_vkDevice), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }
    return shaderModule;
}


std::vector<char> gph::Shader::loadShaderFile(const std::string & filename) {

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
