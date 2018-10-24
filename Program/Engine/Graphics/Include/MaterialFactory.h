#pragma once

#include <MaterialBuilder.h>


namespace Rife::Graphics {

    class MaterialFactory {
    public:

        static Material* defaultMaterial(const std::string& vertShaderName, const std::string& fragShaderName);

    private:

        static std::vector<char> loadShaderFile(const std::string& filename);
        static VkShaderModule createShaderModule(const std::vector<char>& code);
    };
}
