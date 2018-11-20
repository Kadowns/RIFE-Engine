#include <Transform.h>
#include <VulkanTools.h>

namespace Rife::Graphics {

    void Transform::setupBuffer() {

        BufferInfo info = {};
        info.memoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        info.usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        VulkanTools::createBuffer(sizeof(m_ubo), info, m_buffer);
        m_buffer.map();
    }

	void Transform::updateBuffer() {
        m_ubo.model = getModelMatrix();
        m_ubo.inverse = glm::inverse(glm::transpose(m_ubo.model));
		apply();
	}
}

