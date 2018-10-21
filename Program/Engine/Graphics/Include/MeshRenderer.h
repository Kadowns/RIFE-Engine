#pragma once
#include <VkUtilities.h>
#include <Mesh.h>
#include <Renderer.h>

namespace Rife::Graphics {

    class MeshRenderer : public Renderer {

    public:

        void draw();
        void createCommandBuffers();
        void freeCommandBuffers();
        void updateTransformInformation(const glm::mat4& vp, const uint32_t& imageIndex);
        MeshRenderer(Mesh *mesh);
        ~MeshRenderer();

    private:

        void createDescriptorPool();
        void createDescriptorSets();


        Mesh* p_mesh;
        VkBuffer m_vertexBuffer, m_indexBuffer;
        VkDeviceMemory m_vertexBufferMemory, m_indexBufferMemory;
        std::vector<VkBuffer> m_uniformBuffers;
        std::vector<VkDeviceMemory> m_uniformBuffersMemory;
        std::vector<VkDescriptorSet> m_descriptorSets;
        VkDescriptorPool m_descriptorPool;
    };
}