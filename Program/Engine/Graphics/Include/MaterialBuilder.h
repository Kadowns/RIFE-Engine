#pragma once

#include <Material.h>

namespace Rife::Graphics {

    class MaterialBuilder {

    public:

        MaterialBuilder& setShader(Shader* shader);
		MaterialBuilder& setMaterialProperties(Ubo::uMaterialProperties& properties);
        Material* createMaterial();

    private:

		Ubo::uMaterialProperties m_properties;

        Shader* p_shader;
        
    };
}
