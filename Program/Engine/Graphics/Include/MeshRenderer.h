#pragma once
#include <VulkanInclude.h>
#include <Mesh.h>
#include <Renderer.h>
#include <MaterialInstance.h>

namespace Rife::Graphics {

    class MeshRenderer : public Renderer {

    public:

        MeshRenderer(Mesh* mesh, const MaterialInstance& material);
        ~MeshRenderer() {}

        void createCommandBuffers();
        void submitUniformBuffersInfo(const uint32_t& imageIndex);

        void serialize(std::ofstream& file) {

        }

    private:

        Mesh* p_mesh;
       
    };
}