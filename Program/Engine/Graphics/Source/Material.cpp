#include <Material.h>

namespace Rife::Graphics {


    Material::Material(MaterialProperties& properties,
		Shader& shader,
		std::vector<VkDescriptorSetLayoutCreateInfo>& descriptorSetLayoutInfos,
        std::vector<VkPushConstantRange>& pushConstantRangeInfos,
		VkGraphicsPipelineCreateInfo& pipelineCreateInfo
	) {

		m_properties = properties;

		m_shader = shader;
        
        m_clear = false;
    }

    Material::~Material() {
		m_shader.~Shader();
    }

    void Material::clearPipeline() {
        
    }
}
