#pragma once

#include <VkUtilities.h>
#include <RifeCore.h>

namespace Rife::Graphics {

	class Shader : public Base::Object {
	
	public:

		Shader(
			VkGraphicsPipelineCreateInfo&,
			std::vector<VkDescriptorSetLayoutCreateInfo>&,
			std::vector<VkPushConstantRange>&
		);

		~Shader();

		void clearPipeline();

		VkPipeline* getPipeline() { return &m_pipeline; }
		VkPipelineLayout* getPipelineLayout() { return &m_pipelineLayout; }

		std::vector<VkDescriptorSetLayout>* getDescriptorSetLayouts() { return &m_descriptorSetLayouts; }


	private:

		VkPipeline m_pipeline;
		VkPipelineLayout m_pipelineLayout;

		std::vector<VkDescriptorSetLayout> m_descriptorSetLayouts;

	};

}