#include <Material.h>

namespace Rife::Graphics {

    Material::Material(Ubo::uMaterialProperties& properties, Shader* shader, std::vector<Texture*> pTextures) {
        
        m_properties = properties;

        m_pTextures = pTextures;

        p_shader = shader;

        m_clear = false;
    }

    Material::~Material() {
        delete p_shader;
    }

    void Material::clearPipeline() {
        
    }
}
