#pragma once

#include <Material.h>

namespace Rife::Graphics {

    class MaterialBuilder {

    public:

        MaterialBuilder& setShader(Shader* shader);
        MaterialBuilder& addTexture(Texture* texture, uint8_t textureIndex);
        Material* createMaterial();

    private:

        std::vector<Texture*> m_pTextures;

        Shader* p_shader = nullptr;
        
    };
}
