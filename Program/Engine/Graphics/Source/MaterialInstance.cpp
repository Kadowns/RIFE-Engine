#include <MaterialInstance.h>

namespace Rife::Graphics {

    MaterialInstance::MaterialInstance(Material* baseMaterial, Ubo::uMaterialProperties properties) {
        p_baseMaterial = baseMaterial;
        m_properties = properties;
    }
}