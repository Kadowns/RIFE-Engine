#include <ShaderItem.h>
#include <VulkanTools.h>

namespace Rife::Graphics {

    Buffer& ShaderItem::getBuffer() {
        return m_buffer;
    }

	std::vector<ShaderItem*> ShaderItem::s_items;

	void ShaderItem::updateBuffers() {
		for (auto& it : s_items) {
			it->updateBuffer();
		}
	}

    void ShaderItem::flushData(void* data) {
        memcpy(m_buffer.mapped, data, m_buffer.size);
    }
}