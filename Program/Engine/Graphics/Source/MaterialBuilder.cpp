#include <MaterialBuilder.h>

namespace Rife::Graphics {
    
	MaterialBuilder& MaterialBuilder::setMaterialProperties(MaterialProperties& properties) {
		m_properties = properties;
		return *this;
	}
    Material* MaterialBuilder::createMaterial() {
        return new Material(m_properties, m_descriptorSetLayoutInfos, m_pushConstantRanges, m_pipelineInfo);
    }
}


