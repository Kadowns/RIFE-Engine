#include <Shader.h>

#include <VulkanBase.h>
#include <VulkanData.h>
#include <ShaderFactory.h>

namespace Rife::Graphics {

	Shader::Shader(
		std::vector<VkDescriptorSetLayoutCreateInfo>& descriptorSetLayoutInfos,
		std::vector<VkPushConstantRange>& pushConstantRanges,
        std::vector<UniformBufferObjectInfo>& uboInfo,
        std::vector<VkDescriptorSetLayoutBinding>& layoutBindings,
		std::vector<std::string>& filePaths,
        VkVertexInputBindingDescription& vertexBinding,
        std::vector<VkVertexInputAttributeDescription>& vertexAttributes,
        VkPipelineDepthStencilStateCreateInfo& depthStencil,
        VkPipelineColorBlendAttachmentState& colorBlend,
        VkPipelineRasterizationStateCreateInfo& rasterizer
    ) {

        //criação dos descriptorlayout e pipeline layout
		m_descriptorSetLayouts.resize(descriptorSetLayoutInfos.size());
		for (int i = 0; i < m_descriptorSetLayouts.size(); i++) {
			if (vkCreateDescriptorSetLayout(Vulkan::device, &descriptorSetLayoutInfos[i], nullptr, &m_descriptorSetLayouts[i])
				!= VK_SUCCESS) {
				throw std::runtime_error("failed to create descriptor set layout!");
			}
		}

        m_pushConstantRanges = std::move(pushConstantRanges);
		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.pNext = nullptr;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(m_descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = m_descriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(m_pushConstantRanges.size());
		pipelineLayoutInfo.pPushConstantRanges = m_pushConstantRanges.data();

		if (vkCreatePipelineLayout(Vulkan::device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}
        //--------------------------------------------------

        //cache de coisas importantes
        m_name = std::move("Shader");
		m_filePaths = std::move(filePaths);
        m_uboInfo = std::move(uboInfo);
        m_layoutBindings = std::move(layoutBindings);
        m_vertexBinding = std::move(vertexBinding);
        m_vertexAttributes = std::move(vertexAttributes);
        m_colorBlend = std::move(colorBlend);
        m_depthStencil = std::move(depthStencil);
        m_rasterizer = std::move(rasterizer);
        //-------------------------

        //cria a pipeline
		createPipeline();
        //------
        

        //se inscreve nos eventos de resize
        m_pipelineCleanupCallback = [this]() {
            this->clearPipeline();
        };
        VK_BASE->onCleanupPipeline += &m_pipelineCleanupCallback;

        m_pipelineRecreateCallback = [this]() {
            this->createPipeline();
        };
        VK_BASE->onRecreatePipeline += &m_pipelineRecreateCallback;
        //------------------------------------
	}

	Shader::~Shader() {
		clearPipeline();
        vkDestroyPipelineLayout(Vulkan::device, m_pipelineLayout, nullptr);
		for (size_t i = 0; i < m_descriptorSetLayouts.size(); i++) {
			vkDestroyDescriptorSetLayout(Vulkan::device, m_descriptorSetLayouts[i], nullptr);
		}
        VK_BASE->onCleanupPipeline -= &m_pipelineCleanupCallback;
        VK_BASE->onRecreatePipeline -= &m_pipelineRecreateCallback;
	}

	void Shader::createPipeline() {

		std::vector<char> vertShaderCode = ShaderFactory::loadShaderFile(m_filePaths[0]);
		std::vector<char> fragShaderCode = ShaderFactory::loadShaderFile(m_filePaths[1]);

		VkShaderModule vertShaderModule = ShaderFactory::createShaderModule(vertShaderCode);
		VkShaderModule fragShaderModule = ShaderFactory::createShaderModule(fragShaderCode);

		VkPipelineShaderStageCreateInfo vertShaderStageInfo = ShaderFactory::createShaderStage(VK_SHADER_STAGE_VERTEX_BIT, vertShaderModule, "main");
		VkPipelineShaderStageCreateInfo fragShaderStageInfo = ShaderFactory::createShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT, fragShaderModule, "main");

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };


        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)Vulkan::extent.width;
        viewport.height = (float)Vulkan::extent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = {};
        scissor.offset = { 0, 0 };
        scissor.extent = Vulkan::extent;

        VkPipelineViewportStateCreateInfo viewportInfo = ShaderFactory::createViewportInfo(viewport, scissor);

        //input assembly
        VkPipelineInputAssemblyStateCreateInfo inputAssembly = ShaderFactory::createInputAssemblyInfo(
            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            VK_FALSE,
            nullptr
        );
        //--------------

        //multisample
        VkPipelineMultisampleStateCreateInfo multisample = ShaderFactory::createMultisampleInfo();
        //---------------

        //vertex input
        VkPipelineVertexInputStateCreateInfo vertexInputInfo = ShaderFactory::createVertexInputInfo(m_vertexBinding, m_vertexAttributes);
        //---------------

        //color blend
        VkPipelineColorBlendStateCreateInfo colorBlend = ShaderFactory::createColorBlendInfo(m_colorBlend);
        //---------------

        VkGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipelineInfo.basePipelineIndex = -1; // Optional
        pipelineInfo.renderPass = Vulkan::renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.layout = m_pipelineLayout;
        pipelineInfo.pDepthStencilState = &m_depthStencil;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pColorBlendState = &colorBlend;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pRasterizationState = &m_rasterizer;
        pipelineInfo.pMultisampleState = &multisample;
        pipelineInfo.pViewportState = &viewportInfo;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.stageCount = 2;

		if (vkCreateGraphicsPipelines(Vulkan::device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline) != VK_SUCCESS) {
			throw std::runtime_error("failed to create graphics pipeline!");
		}

        vkDestroyShaderModule(Vulkan::device, vertShaderModule, nullptr);
        vkDestroyShaderModule(Vulkan::device, fragShaderModule, nullptr);
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
		vkDestroyPipeline(Vulkan::device, m_pipeline, nullptr);
	}
}
