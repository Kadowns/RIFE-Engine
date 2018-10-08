#include <GraphicsDependencys.h>
#include <fstream>
#include <vector>

namespace gph {

    class Shader {
    public:
        static Shader createShaderInfo();
        static Shader bindDevice(VkDevice* vkdevice);
        static Shader bindShader(const std::string& filename, VkShaderStageFlagBits shaderType);
        static void unbindDevice();
        std::vector<VkPipelineShaderStageCreateInfo>* getShadersInfo();

    private:

        static Shader *m_this;
        Shader() {}
        VkDevice* p_vkDevice;
        std::vector<VkPipelineShaderStageCreateInfo> m_shadersInfo;
        static std::vector<char> loadShaderFile(const std::string& filename);
        static VkShaderModule createShaderModule(const std::vector<char>& code);
    };
}