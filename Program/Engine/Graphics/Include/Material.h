#pragma once
#include <GraphicsLibs.h>
#include <Light.h>
#include <MaterialProperties.h>
#include <VkUtilities.h>
#include <RifeCore.h>
#include <array>

namespace Rife::Graphics {

    class Material : public Base::Object {

    public:

        Material() {}
        Material(
			MaterialProperties&,
			std::vector<VkDescriptorSetLayoutCreateInfo>&,
			std::vector<VkPushConstantRange>&,
			VkGraphicsPipelineCreateInfo&
		);
        ~Material();

        void clearPipeline();

		MaterialProperties& getProperties() { return m_properties; }

        std::vector<VkDescriptorSetLayout>* getDescriptorSetLayouts() { return &m_descriptorSetLayouts; }
        VkDescriptorSet* getDescriptorSet() { return &m_descriptorSet; }

        VkPipelineLayout* getPipelineLayout() { return &m_pipelineLayout; }
        VkPipeline* getPipeline() { return &m_pipeline; }

        bool isCleared() { return m_clear; }

    private:

        bool m_clear;

		MaterialProperties m_properties;

        std::vector<VkDescriptorSetLayout> m_descriptorSetLayouts;
        VkDescriptorSet m_descriptorSet;

        VkPipelineLayout m_pipelineLayout;
        VkPipeline m_pipeline;
    };
}