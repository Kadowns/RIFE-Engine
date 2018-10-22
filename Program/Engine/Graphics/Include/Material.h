#pragma once
#include <GraphicsLibs.h>
#include <VkUtilities.h>
#include <RifeCore.h>
#include <array>

namespace Rife::Graphics {



    class Material : public Base::Object {
    public:

        Material(VkPipelineLayoutCreateInfo& , VkGraphicsPipelineCreateInfo&);
        ~Material();

    private:

        VkDescriptorSetLayout m_descriptorSetLayout;
        VkDescriptorSet m_descriptorSet;

        VkPipelineLayout m_pipelineLayout;
        VkPipeline m_pipeline;
    };
}