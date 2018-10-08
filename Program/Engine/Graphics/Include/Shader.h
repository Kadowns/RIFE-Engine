#include <GraphicsDependencys.h>
#include <fstream>
#include <vector>

namespace gph {

    class ShaderModule {
    public:

        ShaderModule() {}
        
        ShaderModule* bindDevice(VkDevice* vkdevice);
        ShaderModule* bindShader(const std::string& filename, VkShaderStageFlagBits shaderType);
        ShaderModule* unbindDevice();
        std::vector<VkPipelineShaderStageCreateInfo>* getShadersInfo();
        
    private:
        
        VkDevice* p_vkDevice;
        std::vector<VkPipelineShaderStageCreateInfo> m_shadersInfo;
        static std::vector<char> loadShaderFile(const std::string& filename);
        VkShaderModule createShaderModule(const std::vector<char>& code);
    };
}