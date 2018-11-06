#include <MaterialInstance.h>

namespace Rife::Graphics {

    MaterialInstance::MaterialInstance(Material* baseMaterial, Ubo::uMaterialProperties properties) {
        m_name = "MaterialInstance";
        p_baseMaterial = baseMaterial;
        m_properties = properties;
    }
}