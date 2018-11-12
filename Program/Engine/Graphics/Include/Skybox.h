#pragma once

#include <RifeCore.h>
#include <ShaderItem.h>
#include <RifeMath.h>

namespace Rife::Graphics {

    class Skybox : public ShaderItem {

    public:

        static size_t size();

        void serialize(std::ofstream& file) {

        }

        void update();

        void apply(VkDeviceMemory* memory, VkDeviceSize offset);

    private:

        struct {

            glm::mat4 m_projection;
            glm::mat4 m_view;

        } m_ubo;     

    };
}