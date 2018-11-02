#pragma once 

#include <Material.h>
#include <UniformBufferObject.h>

namespace Rife::Graphics {

    class MaterialInstance {

    public:

        MaterialInstance() {}
        MaterialInstance(Material* baseMaterial, Ubo::uMaterialProperties properties);

        Material* getBaseMaterial() { return p_baseMaterial; }
        size_t getTexturesSize() { return p_baseMaterial->getTexturesSize(); }
        std::vector<Texture*>& getTextures() { return p_baseMaterial->getTextures(); }
        Shader* getShader() { return p_baseMaterial->getShader(); }
        Ubo::uMaterialProperties& getProperties() { return m_properties; }

    private:

        Material* p_baseMaterial;
        Ubo::uMaterialProperties m_properties;

    };
}