#pragma once

#include <Texture.h>
#include <ShaderFactory.h>

namespace Rife::Graphics {

    enum MATERIAL_TEXTURE_TYPE {
        MATERIAL_TEXTURE_TYPE_DIFFUSE = 0,
        MATERIAL_TEXTURE_TYPE_SPECULAR = 1
    };

    class Material : public Base::Object {

    public:

        Material() {}
        Material(Ubo::uMaterialProperties&, Shader* shader, Texture* diffuseTex, Texture* specularTex);
        ~Material();

        void clearPipeline();

        Ubo::uMaterialProperties& getProperties() { return m_properties; }

        Shader* getShader() { return p_shader; }

        Texture* getTexture(MATERIAL_TEXTURE_TYPE textureType) { return p_textures[textureType]; }

        size_t getTexturesSize() { return p_textures.size(); }

        bool isCleared() { return m_clear; }

    private:

        bool m_clear;

        Ubo::uMaterialProperties m_properties;
        std::vector<Texture*> p_textures;
		Shader* p_shader;

    };
}