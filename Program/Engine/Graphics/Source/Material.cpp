#include <Material.h>

namespace Rife::Graphics {

    Material::Material(Ubo::uMaterialProperties& properties, Shader* shader, Texture* diffuseTex, Texture* specularTex) {
        
        m_properties = properties;

        p_textures.push_back(diffuseTex);
        p_textures.push_back(specularTex);

        p_shader = shader;

        m_clear = false;
    }

    Material::~Material() {
        delete p_shader;
    }

    void Material::clearPipeline() {
        
    }
}
