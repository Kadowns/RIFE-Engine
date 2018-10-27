#pragma once
#include <Light.h>
#include <ShaderFactory.h>
#include <MaterialProperties.h>

namespace Rife::Graphics {

    class Material : public Base::Object {

    public:

        Material() {}
        Material(MaterialProperties&, Shader* shader);
        ~Material();

        void clearPipeline();

		MaterialProperties& getProperties() { return m_properties; }

		Shader* getShader() { return p_shader; }

        bool isCleared() { return m_clear; }

    private:

        bool m_clear;

		MaterialProperties m_properties;
		Shader* p_shader;

    };
}