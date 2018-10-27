#pragma once

#include <MaterialBuilder.h>
#include <ShaderFactory.h>


namespace Rife::Graphics {

    class MaterialFactory {
    public:

        static Material* defaultMaterial(const std::string& vertShaderName, const std::string& fragShaderName);

    };
}
