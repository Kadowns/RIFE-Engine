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
        Buffer buffer;
    };

	class Shader : public Base::Object {
	
	public:

		Shader(
			VkGraphicsPipelineCreateInfo&,
			std::vector<VkDescriptorSetLayoutCreateInfo>&,
			std::vector<VkPushConstantRange>&,
            std::vector<UniformBufferObjectInfo>&,
            std::vector<VkDescriptorSetLayoutBinding>&
		);
        Shader() { m_name = "Shader"; }

        void serialize(std::ofstream& file) {
            file << (m_name + "\n").c_str();
        }

		~Shader();

        Shader& bindUniformBufferMemory(VkDeviceMemory* memory);
        Shader& setItem(ShaderItem& item);

		void clearPipeline();
		VkPipeline* getPipeline() { return &m_pipeline; }
		VkPipelineLayout* getPipelineLayout() { return &m_pipelineLayout; }

        UniformBufferObjectInfo getUboInfo(size_t index);
        size_t getUboSize();
        VkDeviceSize getUboOffset(size_t uboIndex);

        std::vector<VkDescriptorSetLayoutBinding>& getLayoutBindings() { return m_layoutBindings; }
		std::vector<VkDescriptorSetLayout>& getDescriptorSetLayouts() { return m_descriptorSetLayouts; }

	private:

        VkDeviceMemory* p_uniformBufferMemory = nullptr;
        VkDeviceSize m_uboOffset;
        size_t m_itemIndex;

		VkPipeline m_pipeline;
		VkPipelineLayout m_pipelineLayout;

        std::vector<UniformBufferObjectInfo> m_uboInfo;
        std::vector<VkDescriptorSetLayoutBinding> m_layoutBindings;
		std::vector<VkDescriptorSetLayout> m_descriptorSetLayouts;

	};


}