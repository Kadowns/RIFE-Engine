#include <Material.h>

namespace Rife::Graphics {


    Material::Material(MaterialProperties& properties, Shader* shader) {

		m_properties = properties;

		p_shader = shader;
        
        m_clear = false;
    }

    Material::~Material() {
        delete p_shader;
    }

    void Material::clearPipeline() {
        
    }
}
