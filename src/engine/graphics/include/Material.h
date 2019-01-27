#pragma once

#include <Texture.h>
#include <ShaderFactory.h>

namespace Rife::Graphics {

    class Material : public Base::Object {

    public:

        Material() { m_name = "Material"; }
        Material(Shader* shader, std::vector<Texture*> pTextures);
        ~Material();

        void clearPipeline();

        Shader* getShader() { return p_shader; }

        std::vector<Texture*>& getTextures() { return m_pTextures; }

        size_t getTexturesSize() { return m_pTextures.size(); }

        bool isCleared() { return m_clear; }

        void serialize(std::ofstream& file) {
            file << (m_name + "\n").c_str();
        }

    private:

        bool m_clear;

        std::vector<Texture*> m_pTextures;
		Shader* p_shader;

    };
}