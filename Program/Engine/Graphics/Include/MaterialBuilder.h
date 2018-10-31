#pragma once

#include <Material.h>

namespace Rife::Graphics {

    class MaterialBuilder {

    public:

        MaterialBuilder& setShader(Shader* shader);
		MaterialBuilder& setMaterialProperties(Ubo::uMaterialProperties& properties);
        MaterialBuilder& addTexture(Texture* texture, MATERIAL_TEXTURE_TYPE textureType);
        Material* createMaterial();

    private:

		Ubo::uMaterialProperties m_properties;

        std::vector<Texture*> m_pTextures;

        Shader* p_shader = nullptr;
        
    };
}
