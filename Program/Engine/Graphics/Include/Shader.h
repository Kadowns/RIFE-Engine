#pragma once

#include <VulkanInclude.h>
#include <RifeCore.h>
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
			VkGraphicsPipelineCreateInfo&,
			std::vector<VkDescriptorSetLayoutCreateInfo>&,
			std::vector<VkPushConstantRange>&,
			std::vector<UniformBufferObjectInfo>&,
			std::vector<VkDescriptorSetLayoutBinding>&,
			std::vector<std::string>& shaderNames,
			VkPipelineViewportStateCreateInfo& viewportInfo
		);
        Shader() { m_name = "Shader"; }

        void serialize(std::ofstream& file) {
            file << (m_name + "\n").c_str();
        }

		~Shader();

		void clearPipeline();
		VkPipeline* getPipeline() { return &m_pipeline; }
		VkPipelineLayout* getPipelineLayout() { return &m_pipelineLayout; }
		void createPipeline(VkPipelineViewportStateCreateInfo& viewportInfo);

        UniformBufferObjectInfo getUboInfo(size_t index);
        size_t getUboSize();
        VkDeviceSize getUboOffset(size_t uboIndex);

        std::vector<VkDescriptorSetLayoutBinding>& getLayoutBindings() { return m_layoutBindings; }
		std::vector<VkDescriptorSetLayout>& getDescriptorSetLayouts() { return m_descriptorSetLayouts; }

		

	private:

		VkGraphicsPipelineCreateInfo m_pipelineInfo;
		VkPipeline m_pipeline;
		VkPipelineLayout m_pipelineLayout;
		std::vector<std::string> m_shaderNames;

        std::vector<UniformBufferObjectInfo> m_uboInfo;
        std::vector<VkDescriptorSetLayoutBinding> m_layoutBindings;
		std::vector<VkDescriptorSetLayout> m_descriptorSetLayouts;

	};


}