#include <Shader.h>
#include <VulkanData.h>

namespace Rife::Graphics {

	Shader::Shader(VkGraphicsPipelineCreateInfo & pipelineCreateInfo,
		std::vector<VkDescriptorSetLayoutCreateInfo>& descriptorSetLayoutInfos,
		std::vector<VkPushConstantRange>& pushConstantRanges,
        std::vector<UniformBufferObjectInfo>& uboInfo,
        std::vector<VkDescriptorSetLayoutBinding>& layoutBindings
    ) {

		m_descriptorSetLayouts.resize(descriptorSetLayoutInfos.size());
		for (int i = 0; i < m_descriptorSetLayouts.size(); i++) {
			if (vkCreateDescriptorSetLayout(VK_DATA->getDevice(), &descriptorSetLayoutInfos[i], nullptr, &m_descriptorSetLayouts[i])
				!= VK_SUCCESS) {
				throw std::runtime_error("failed to create descriptor set layout!");
			}
		}

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.pNext = nullptr;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(m_descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = m_descriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(pushConstantRanges.size());
		pipelineLayoutInfo.pPushConstantRanges = pushConstantRanges.data();

		if (vkCreatePipelineLayout(VK_DATA->getDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}

		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		pipelineCreateInfo.basePipelineIndex = -1; // Optional
		pipelineCreateInfo.renderPass = VK_DATA->getRenderPass();
		pipelineCreateInfo.subpass = 0;
		pipelineCreateInfo.layout = m_pipelineLayout;


		if (vkCreateGraphicsPipelines(VK_DATA->getDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &m_pipeline) != VK_SUCCESS) {
			throw std::runtime_error("failed to create graphics pipeline!");
		}

        m_name = "Shader";
        m_uboInfo = uboInfo;
        m_layoutBindings = layoutBindings;
	}

	Shader::~Shader() {
		clearPipeline();
		for (size_t i = 0; i < m_descriptorSetLayouts.size(); i++) {
			vkDestroyDescriptorSetLayout(VK_DATA->getDevice(), m_descriptorSetLayouts[i], nullptr);
		}
	}


    Shader& Shader::bindUniformBufferMemory(VkDeviceMemory* memory) {
        p_uniformBufferMemory = memory;
        m_uboOffset = 0;
        m_itemIndex = 0;
        return *this;
    }

    UniformBufferObjectInfo Shader::getUboInfo(size_t index) {
        return m_uboInfo[index];
    }

    VkDeviceSize Shader::getUboOffset(size_t uboIndex) {
        VkDeviceSize sum = 0;
        for (size_t i = 0; i < uboIndex; i++) {
            sum += m_uboInfo[i].offset;
        }
        return sum;
    }

    size_t Shader::getUboSize() {
        return m_uboInfo.size();
    }

    Shader& Shader::setItem(ShaderItem& item) {
        item.apply(p_uniformBufferMemory, m_uboOffset);
        m_itemIndex++;
        m_uboOffset += m_uboInfo[m_itemIndex - 1].offset;
        return *this;
    }

	void Shader::clearPipeline() {
		vkDestroyPipeline(VK_DATA->getDevice(), m_pipeline, nullptr);
		vkDestroyPipelineLayout(VK_DATA->getDevice(), m_pipelineLayout, nullptr);
	}
}
