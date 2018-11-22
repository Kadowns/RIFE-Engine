#include <Shader.h>
#include <VulkanData.h>
#include <ShaderFactory.h>

namespace Rife::Graphics {

	Shader::Shader(VkGraphicsPipelineCreateInfo& pipelineCreateInfo,
		std::vector<VkDescriptorSetLayoutCreateInfo>& descriptorSetLayoutInfos,
		std::vector<VkPushConstantRange>& pushConstantRanges,
        std::vector<UniformBufferObjectInfo>& uboInfo,
        std::vector<VkDescriptorSetLayoutBinding>& layoutBindings,
		std::vector<std::string>& shaderNames,
		VkPipelineViewportStateCreateInfo& viewportInfo
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

		m_shaderNames = std::move(shaderNames);
		m_pipelineInfo = std::move(pipelineCreateInfo);
		createPipeline(viewportInfo);
		

        m_name = std::move("Shader");
        m_uboInfo = std::move(uboInfo);
        m_layoutBindings = std::move(layoutBindings);
	}

	Shader::~Shader() {
		clearPipeline();
		for (size_t i = 0; i < m_descriptorSetLayouts.size(); i++) {
			vkDestroyDescriptorSetLayout(VK_DATA->getDevice(), m_descriptorSetLayouts[i], nullptr);
		}
	}

	void Shader::createPipeline(VkPipelineViewportStateCreateInfo& viewportInfo) {

		m_pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		m_pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		m_pipelineInfo.basePipelineIndex = -1; // Optional
		m_pipelineInfo.renderPass = VK_DATA->getRenderPass();
		m_pipelineInfo.subpass = 0;
		m_pipelineInfo.layout = m_pipelineLayout;
		m_pipelineInfo.pViewportState = &viewportInfo;

		std::vector<char> vertShaderCode = ShaderFactory::loadShaderFile(m_shaderNames[0]);
		std::vector<char> fragShaderCode = ShaderFactory::loadShaderFile(m_shaderNames[1]);

		VkShaderModule vertShaderModule = ShaderFactory::createShaderModule(vertShaderCode);
		VkShaderModule fragShaderModule = ShaderFactory::createShaderModule(fragShaderCode);

		VkPipelineShaderStageCreateInfo vertShaderStageInfo = ShaderFactory::createShaderStage(VK_SHADER_STAGE_VERTEX_BIT, vertShaderModule, "main");
		VkPipelineShaderStageCreateInfo fragShaderStageInfo = ShaderFactory::createShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT, fragShaderModule, "main");

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };
		m_pipelineInfo.pStages = shaderStages;
		m_pipelineInfo.stageCount = 2;

		if (vkCreateGraphicsPipelines(VK_DATA->getDevice(), VK_NULL_HANDLE, 1, &m_pipelineInfo, nullptr, &m_pipeline) != VK_SUCCESS) {
			throw std::runtime_error("failed to create graphics pipeline!");
		}
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

	void Shader::clearPipeline() {
		vkDestroyPipeline(VK_DATA->getDevice(), m_pipeline, nullptr);
		vkDestroyPipelineLayout(VK_DATA->getDevice(), m_pipelineLayout, nullptr);
	}
}
