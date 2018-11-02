#pragma once
#include <VulkanInclude.h>
#include <Mesh.h>
#include <Renderer.h>
#include <MaterialInstance.h>

namespace Rife::Graphics {

    class MeshRenderer : public Renderer {

    public:

        void createCommandBuffers();
        void freeCommandBuffers();
        void submitUniformBuffersInfo(const uint32_t& imageIndex);
        MeshRenderer(Mesh* mesh, const MaterialInstance& material);
        ~MeshRenderer();

    private:

        void createDescriptorPool();
        void createDescriptorSets();

        Ubo::uTransform m_ubo;

        Mesh* p_mesh;
        MaterialInstance m_material;

        std::vector<VkBuffer> m_uniformBuffers;
        std::vector<VkDeviceMemory> m_uniformBuffersMemory;
        std::vector<VkDescriptorSet> m_descriptorSets;
        VkDescriptorPool m_descriptorPool;
    };
}