#include <MaterialBuilder.h>

namespace Rife::Graphics {
    
    MaterialBuilder& MaterialBuilder::setShader(Shader* shader) {
        p_shader = shader;
        return *this;
    }

    MaterialBuilder& MaterialBuilder::addTexture(Texture* texture, uint8_t textureIndex) {
        if (m_pTextures.size() <= textureIndex) {
            m_pTextures.resize(textureIndex + 1);
        }
        m_pTextures[textureIndex] = texture;
        return *this;
    }

    Material* MaterialBuilder::createMaterial() {
        return new Material(p_shader, m_pTextures);
    }
}


