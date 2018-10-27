#pragma once
#include <Light.h>
#include <ShaderFactory.h>
#include <MaterialProperties.h>

namespace Rife::Graphics {

    class Material : public Base::Object {

    public:

        Material() {}
        Material(
			MaterialProperties&,
			Shader& shader,
			std::vector<VkDescriptorSetLayoutCreateInfo>&,
			std::vector<VkPushConstantRange>&,
			VkGraphicsPipelineCreateInfo&
		);
        ~Material();

        void clearPipeline();

		MaterialProperties& getProperties() { return m_properties; }

		Shader* getShader() { return &m_shader; }

        bool isCleared() { return m_clear; }

    private:

        bool m_clear;

		MaterialProperties m_properties;
		Shader m_shader;

    };
}