#include <Skybox.h>
#include <VulkanTools.h>

#include <Camera.h>
#include <Transform.h>

#define SKYBOX Rife::Graphics::Skybox::getInstance()


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

    void Skybox::setupBuffer() {

        BufferInfo info = {};
        info.memoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        info.usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        m_buffers.resize(VK_DATA->getSwapchainImages().size());
        for (size_t i = 0; i < m_buffers.size(); i++) {
            VulkanTools::createBuffer(sizeof(m_ubo), info, m_buffers[i]);
            m_buffers[i].map();
        }
    }

	void Skybox::updateBuffer(uint32_t imageIndex) {
		m_ubo.m_projection = CAMERA->getProjection();
		m_ubo.m_view = glm::mat3(CAMERA->getView());
        flushData(&m_ubo, imageIndex);
	}
}
