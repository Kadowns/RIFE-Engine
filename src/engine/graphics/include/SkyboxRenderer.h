#pragma once

#include <Renderer.h>
#include <Skybox.h>
#include <Mesh.h>

namespace Rife::Graphics {

    class SkyboxRenderer : public Renderer {

    public:

        SkyboxRenderer(Mesh* mesh, const MaterialInstance& material);
        ~SkyboxRenderer();

        void createCommandBuffers();

        void serialize(std::ofstream& file) {

        }

    private:

        Skybox m_skybox;
        Mesh* p_mesh;

    };

}