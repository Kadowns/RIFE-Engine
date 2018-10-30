#pragma once

#include <Material.h>

namespace Rife::Graphics {

    class MaterialBuilder {

    public:

        MaterialBuilder& setShader(Shader* shader);
		MaterialBuilder& setMaterialProperties(Ubo::uMaterialProperties& properties);
        MaterialBuilder& setDiffuseTexture(Texture* texture);
        MaterialBuilder& setSpecularTexture(Texture* texture);
        Material* createMaterial();

    private:

		Ubo::uMaterialProperties m_properties;

        Texture* p_texDiffuse = nullptr;
        Texture* p_texSpecular = nullptr;

        Shader* p_shader = nullptr;
        
    };
}
