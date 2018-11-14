#include <Skybox.h>
#include <VulkanTools.h>

#include <Camera.h>
#include <Transform.h>


namespace Rife::Graphics {

    Skybox::Skybox() {
        setupBuffer();
    }

    Skybox* Skybox::getInstance() {
        static Skybox* instance = new Skybox();
        return instance;
    }

    size_t Skybox::size() {
        return sizeof(m_ubo);
    }

    void Skybox::update() {

        m_ubo.m_projection = CAMERA->getProjection();

        m_ubo.m_view = glm::mat3(CAMERA->getView());

    }

    void Skybox::apply(VkDeviceMemory* memory, VkDeviceSize offset) {
        flushData(&m_ubo);
    }

    void Skybox::setupBuffer() {

        BufferInfo info = {};
        info.memoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        info.usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        VulkanTools::createBuffer(sizeof(m_ubo), info, m_buffer);
        m_buffer.map();
    }

}
