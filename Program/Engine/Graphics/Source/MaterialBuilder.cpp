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

    MaterialBuilder& MaterialBuilder::setDiffuseTexture(Texture* texture) {
        p_texDiffuse = texture;
        return *this;
    }

    MaterialBuilder& MaterialBuilder::setSpecularTexture(Texture* texture) {
        p_texSpecular = texture;
        return *this;
    }

    Material* MaterialBuilder::createMaterial() {
        return new Material(m_properties, p_shader, p_texDiffuse, p_texSpecular);
    }
}


