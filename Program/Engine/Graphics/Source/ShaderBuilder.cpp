#include <ShaderBuilder.h>

namespace Rife::Graphics {

	ShaderBuilder& ShaderBuilder::addDescriptorSetLayoutInfo(VkDescriptorSetLayoutCreateInfo& descriptorLayoutInfo) {
		m_descriptorSetLayoutInfos.push_back(descriptorLayoutInfo);
		return *this;
	}

	ShaderBuilder& ShaderBuilder::addPushConstantRange(VkPushConstantRange& pushConstantRange) {
		m_pushConstantRanges.push_back(pushConstantRange);
		return *this;
	}

    ShaderBuilder& ShaderBuilder::addUniformBufferObjectInfo(UniformBufferObjectInfo& uboInfo) {
        m_uboInfo.push_back(uboInfo);
        return *this;
    }

    ShaderBuilder& ShaderBuilder::setLayoutBindings(std::vector<VkDescriptorSetLayoutBinding>& layoutBindings) {
        m_layoutBindings = layoutBindings;
        return *this;
    }

    ShaderBuilder & ShaderBuilder::setShaderNames(std::vector<std::string>& names) {
        m_names = std::move(names);
        return *this;
    }

    ShaderBuilder& ShaderBuilder::setRasterizer(VkPipelineRasterizationStateCreateInfo& rasterizer) {
        m_rasterizer = std::move(rasterizer);
        return *this;
    }

    ShaderBuilder& ShaderBuilder::setVertexInputBinding(VkVertexInputBindingDescription& vertexInputBinding) {
        m_vertexBinding = std::move(vertexInputBinding);
        return *this;
    }

    ShaderBuilder& ShaderBuilder::setVertexAttribute(std::vector<VkVertexInputAttributeDescription>& vertexAttribute) {
        m_vertexAttributes = std::move(vertexAttribute);
        return *this;
    }

	ShaderBuilder& ShaderBuilder::setDepthStencil(VkPipelineDepthStencilStateCreateInfo& depthStencil) {
		m_depthStencil = std::move(depthStencil);
		return *this;
	}

	ShaderBuilder& ShaderBuilder::setColorBlend(VkPipelineColorBlendAttachmentState& colorBlending) {
        m_colorBlend = std::move(colorBlending);
		return *this;
	}

	Shader* ShaderBuilder::createShader() {
        return new Shader(
            m_descriptorSetLayoutInfos,
            m_pushConstantRanges,
            m_uboInfo,
            m_layoutBindings,
            m_names,
            m_vertexBinding,
            m_vertexAttributes,
            m_depthStencil,
            m_colorBlend,
            m_rasterizer
        );
	}
}