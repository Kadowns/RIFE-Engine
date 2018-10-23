#pragma once

#include <Material.h>

namespace Rife::Graphics {

    class MaterialBuilder {

    public:

        MaterialBuilder& addDescriptorSetLayoutInfo(VkDescriptorSetLayoutCreateInfo& descriptorLayoutInfo);
        MaterialBuilder& addPushConstantRange(VkPushConstantRange& pushConstantRange);
        MaterialBuilder& setShaderStages(VkPipelineShaderStageCreateInfo* shaderStages, const uint32_t& stageCount);
        MaterialBuilder& setVertexInputState(VkPipelineVertexInputStateCreateInfo& vertexInputInfo);
        MaterialBuilder& setInputAssemblyState(VkPipelineInputAssemblyStateCreateInfo& inputAssembly);
        MaterialBuilder& setViewportState(VkPipelineViewportStateCreateInfo& viewportState);
        MaterialBuilder& setRasterizationState(VkPipelineRasterizationStateCreateInfo& rasterizer);
        MaterialBuilder& setMultisampleState(VkPipelineMultisampleStateCreateInfo& multisampling);
        MaterialBuilder& setDepthStencilState(VkPipelineDepthStencilStateCreateInfo& depthStencil);
        MaterialBuilder& setColorBlendState(VkPipelineColorBlendStateCreateInfo& colorBlending);
        Material* createMaterial();

    private:

        std::vector<VkDescriptorSetLayoutCreateInfo> m_descriptorSetLayoutInfos;
        std::vector<VkPushConstantRange> m_pushConstantRanges;
        VkGraphicsPipelineCreateInfo m_pipelineInfo;
    };
}
