#pragma once
#include <GraphicsLibs.h>
#include <Light.h>
#include <VkUtilities.h>
#include <RifeCore.h>
#include <array>

namespace Rife::Graphics {

    class Material : public Base::Object {

    public:

        Material() {}
        Material(std::vector<VkDescriptorSetLayoutCreateInfo>&, std::vector<VkPushConstantRange>&, VkGraphicsPipelineCreateInfo&);
        ~Material();

        void clearPipeline();

        std::vector<VkDescriptorSetLayout>* getDescriptorSetLayouts() { return &m_descriptorSetLayouts; }
        VkDescriptorSet* getDescriptorSet() { return &m_descriptorSet; }

        VkPipelineLayout* getPipelineLayout() { return &m_pipelineLayout; }
        VkPipeline* getPipeline() { return &m_pipeline; }

        bool isCleared() { return m_clear; }

    private:

        bool m_clear;

        std::vector<VkDescriptorSetLayout> m_descriptorSetLayouts;
        VkDescriptorSet m_descriptorSet;

        VkPipelineLayout m_pipelineLayout;
        VkPipeline m_pipeline;
    };
}