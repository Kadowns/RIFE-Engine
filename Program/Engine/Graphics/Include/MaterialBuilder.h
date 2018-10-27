#pragma once

#include <Material.h>

namespace Rife::Graphics {

    class MaterialBuilder {

    public:

        
		MaterialBuilder& setMaterialProperties(MaterialProperties& properties);
        Material* createMaterial();

    private:

		MaterialProperties m_properties;
        
    };
}
