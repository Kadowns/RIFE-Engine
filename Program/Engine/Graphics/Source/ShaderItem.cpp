#include <ShaderItem.h>
#include <VulkanTools.h>

namespace Rife::Graphics {

    Buffer& ShaderItem::getBuffer() {
        return m_buffer;
    }

	std::vector<ShaderItem*> ShaderItem::s_items;

	void ShaderItem::updateUniformBuffers() {
		for (auto& it : s_items) {
			it->updateUniformBuffer();
		}
	}

    void ShaderItem::flushData(void* data) {
        memcpy(m_buffer.mapped, data, m_buffer.size);
    }
}