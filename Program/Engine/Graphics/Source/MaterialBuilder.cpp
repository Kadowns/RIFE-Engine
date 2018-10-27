#include <MaterialBuilder.h>

namespace Rife::Graphics {
    
    MaterialBuilder& MaterialBuilder::setShader(Shader* shader) {
        p_shader = shader;
        return *this;
    }

	MaterialBuilder& MaterialBuilder::setMaterialProperties(MaterialProperties& properties) {
		m_properties = properties;
		return *this;
	}
    Material* MaterialBuilder::createMaterial() {
        return new Material(m_properties, p_shader);
    }
}


