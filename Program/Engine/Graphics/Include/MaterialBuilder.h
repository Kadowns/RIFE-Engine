#pragma once

#include <Material.h>

namespace Rife::Graphics {

    class MaterialBuilder {

    public:

        MaterialBuilder& setShader(Shader* shader);
        MaterialBuilder& addTexture(Texture* texture, MATERIAL_TEXTURE_TYPE textureType);
        Material* createMaterial();

    private:

        std::vector<Texture*> m_pTextures;

        Shader* p_shader = nullptr;
        
    };
}
