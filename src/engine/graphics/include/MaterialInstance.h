#pragma once 

#include <Material.h>
#include <UniformBufferObject.h>

namespace Rife::Graphics {

    class MaterialInstance : public Base::Object{

    public:

        MaterialInstance() { m_name = "MaterialInstance"; }
        MaterialInstance(Material* baseMaterial, Ubo::MaterialProperties properties);

        Material* getBaseMaterial() { return p_baseMaterial; }
        size_t getTexturesSize() { return p_baseMaterial->getTexturesSize(); }
        std::vector<Texture*>& getTextures() { return p_baseMaterial->getTextures(); }
        Shader* getShader() { return p_baseMaterial->getShader(); }
        Ubo::MaterialProperties& getProperties() { return m_properties; }

        void serialize(std::ofstream& file) {
            file << (m_name + "\n").c_str();
        }

    private:

        Material* p_baseMaterial;
        Ubo::MaterialProperties m_properties;

    };
}