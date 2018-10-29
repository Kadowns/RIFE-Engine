#pragma once

#include <ShaderFactory.h>

namespace Rife::Graphics {

    class Material : public Base::Object {

    public:

        Material() {}
        Material(Ubo::uMaterialProperties&, Shader* shader);
        ~Material();

        void clearPipeline();

        Ubo::uMaterialProperties& getProperties() { return m_properties; }

		Shader* getShader() { return p_shader; }

        bool isCleared() { return m_clear; }

    private:

        bool m_clear;

        Ubo::uMaterialProperties m_properties;
		Shader* p_shader;

    };
}