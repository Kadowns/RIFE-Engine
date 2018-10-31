#include <MaterialBuilder.h>

namespace Rife::Graphics {
    
    MaterialBuilder& MaterialBuilder::setShader(Shader* shader) {
        p_shader = shader;
        return *this;
    }

	MaterialBuilder& MaterialBuilder::setMaterialProperties(Ubo::uMaterialProperties& properties) {
		m_properties = properties;
		return *this;
	}

    MaterialBuilder& MaterialBuilder::addTexture(Texture* texture, MATERIAL_TEXTURE_TYPE textureType) {
        if (m_pTextures.size() <= textureType) {
            m_pTextures.resize(textureType + 1);
        }
        m_pTextures[textureType] = texture;
        return *this;
    }

    Material* MaterialBuilder::createMaterial() {
        return new Material(m_properties, p_shader, m_pTextures);
    }
}


