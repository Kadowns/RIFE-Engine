#include <Material.h>

namespace Rife::Graphics {

    Material::Material(Shader* shader, std::vector<Texture*> pTextures) {

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
