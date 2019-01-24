#include <Transform.h>
#include <VulkanTools.h>
#include <VulkanData.h>

namespace Rife::Graphics {

    void Transform::setupBuffer() {

        BufferInfo info = {};
        info.memoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        info.usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

        m_buffers.resize(Vulkan::swapchainImages.size());
        for (size_t i = 0; i < m_buffers.size(); i++) {
            VulkanTools::createBuffer(sizeof(m_ubo), info, m_buffers[i]);
            m_buffers[i].map();
        }
    }

	void Transform::updateBuffer(uint32_t imageIndex) {
        m_ubo.model = getModelMatrix();
        m_ubo.inverse = glm::inverse(glm::transpose(m_ubo.model));
        flushData(&m_ubo, imageIndex);
	}
}

