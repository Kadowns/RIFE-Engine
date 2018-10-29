#pragma once
#include <VkUtilities.h>
#include <Mesh.h>
#include <MaterialFactory.h>
#include <Camera.h>
#include <Renderer.h>

namespace Rife::Graphics {

    class MeshRenderer : public Renderer {

    public:

        void draw();
        void createCommandBuffers();
        void freeCommandBuffers();
        void updateUbos(const uint32_t& imageIndex);
        MeshRenderer(Mesh* mesh, Material* material);
        ~MeshRenderer();

    private:

        void createDescriptorPool();
        void createDescriptorSets();

        Ubo::uTransform m_ubo;

        Mesh* p_mesh;
        Material* p_material;

        std::vector<VkBuffer> m_uniformBuffers;
        std::vector<VkDeviceMemory> m_uniformBuffersMemory;
        std::vector<VkDescriptorSet> m_descriptorSets;
        VkDescriptorPool m_descriptorPool;
    };
}