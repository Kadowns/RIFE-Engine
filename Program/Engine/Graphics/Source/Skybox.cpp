#include <Skybox.h>
#include <Camera.h>
#include <Transform.h>

namespace Rife::Graphics {

    size_t Skybox::size() {
        return sizeof(m_ubo);
    }

    void Skybox::update() {

        m_ubo.m_projection = CAMERA->getProjection();

        m_ubo.m_view = glm::mat3(CAMERA->getView());

    }

    void Skybox::apply(VkDeviceMemory* memory, VkDeviceSize offset) {
        flushData(memory, size(), offset, &m_ubo);
    }

}
