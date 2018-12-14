#pragma once

#include <RifeCore.h>
#include <VulkanInclude.h>

#include <Buffer.h>
#include <UniformBufferObject.h>

namespace Rife::Graphics {

    struct UniformBufferObjectInfo {
        std::vector<void*> pDatas;
        VkDeviceSize offset;
        VkDeviceSize range;
		SHADER_ITEM_TYPE type;
    };

	class Shader : public Base::Object {
	
	public:

        Shader(
            std::vector<VkDescriptorSetLayoutCreateInfo>&,
            std::vector<VkPushConstantRange>&,
            std::vector<UniformBufferObjectInfo>&,
            std::vector<VkDescriptorSetLayoutBinding>&,
            std::vector<std::string>& filePaths,
            VkVertexInputBindingDescription& vertexBinding,
            std::vector<VkVertexInputAttributeDescription>& vertexAttributes,
            VkPipelineDepthStencilStateCreateInfo& depthStencil,
            VkPipelineColorBlendAttachmentState& colorBlend,
            VkPipelineRasterizationStateCreateInfo& rasterizer
		);
        Shader() { m_name = "Shader"; }

        void serialize(std::ofstream& file) {
            file << (m_name + "\n").c_str();
        }

		~Shader();

		void clearPipeline();
		VkPipeline* getPipeline() { return &m_pipeline; }
		VkPipelineLayout* getPipelineLayout() { return &m_pipelineLayout; }
		void createPipeline();

        UniformBufferObjectInfo getUboInfo(size_t index);
        size_t getUboSize();
        VkDeviceSize getUboOffset(size_t uboIndex);

        std::vector<VkDescriptorSetLayoutBinding>& getLayoutBindings() { return m_layoutBindings; }
		std::vector<VkDescriptorSetLayout>& getDescriptorSetLayouts() { return m_descriptorSetLayouts; }

		

	private:

		
		VkPipeline m_pipeline;
		VkPipelineLayout m_pipelineLayout;
        VkVertexInputBindingDescription m_vertexBinding;
        std::vector<VkVertexInputAttributeDescription> m_vertexAttributes;
        VkPipelineDepthStencilStateCreateInfo m_depthStencil;
        VkPipelineColorBlendAttachmentState m_colorBlend;
        VkPipelineRasterizationStateCreateInfo m_rasterizer;

		std::vector<std::string> m_filePaths;
        std::vector<UniformBufferObjectInfo> m_uboInfo;
        std::vector<VkDescriptorSetLayoutBinding> m_layoutBindings;
		std::vector<VkDescriptorSetLayout> m_descriptorSetLayouts;
        std::vector<VkPushConstantRange> m_pushConstantRanges;

        OnCleanupPipeline::EventListener m_pipelineCleanupCallback;
        OnRecreatePipeline::EventListener m_pipelineRecreateCallback;


	};


}