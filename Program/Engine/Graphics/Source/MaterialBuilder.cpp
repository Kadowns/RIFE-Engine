#include <MaterialBuilder.h>

namespace Rife::Graphics {

    MaterialBuilder* MaterialBuilder::setShaderStages(VkPipelineShaderStageCreateInfo* shaderStages, const uint32_t& stageCount) {
        m_pipelineInfo.stageCount = stageCount;
        m_pipelineInfo.pStages = shaderStages;
        return this;
    }

    MaterialBuilder* MaterialBuilder::setVertexInputState(VkPipelineVertexInputStateCreateInfo& vertexInputInfo) {
        m_pipelineInfo.pVertexInputState = &vertexInputInfo;
        return this;
    }
    MaterialBuilder* MaterialBuilder::setInputAssemblyState(VkPipelineInputAssemblyStateCreateInfo& inputAssembly) {
        m_pipelineInfo.pInputAssemblyState = &inputAssembly;
        return this;
    }
    MaterialBuilder* MaterialBuilder::setViewportState(VkPipelineViewportStateCreateInfo& viewportState) {
        m_pipelineInfo.pViewportState = &viewportState;
        return this;
    }
    MaterialBuilder* MaterialBuilder::setRasterizationState(VkPipelineRasterizationStateCreateInfo& rasterizer) {
        m_pipelineInfo.pRasterizationState = &rasterizer;
        return this;
    }
    MaterialBuilder* MaterialBuilder::setMultisampleState(VkPipelineMultisampleStateCreateInfo& multisampling) {
        m_pipelineInfo.pMultisampleState = &multisampling;
        return this;
    }
    MaterialBuilder* MaterialBuilder::setDepthStencilState(VkPipelineDepthStencilStateCreateInfo& depthStencil) {
        m_pipelineInfo.pDepthStencilState = &depthStencil;
        return this;
    }
    MaterialBuilder* MaterialBuilder::setColorBlendState(VkPipelineColorBlendStateCreateInfo& colorBlending) {
        m_pipelineInfo.pColorBlendState = &colorBlending;
        return this;
    }
    Material MaterialBuilder::createMaterial() {
        m_pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        m_pipelineInfo.renderPass = *VK_WRAPPER->getRenderPass();
        m_pipelineInfo.subpass = 0;
        m_pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        m_pipelineInfo.basePipelineIndex = -1; // Optional
        return Material();
    }
}


