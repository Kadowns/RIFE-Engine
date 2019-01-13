#include <ShaderItem.h>
#include <VulkanTools.h>

namespace Rife::Graphics {

    Buffer& ShaderItem::getBuffer(uint32_t index) {
        return m_buffers[index];
    }

	std::vector<ShaderItem*> ShaderItem::s_items;

	void ShaderItem::updateBuffers(uint32_t index) {
		for (auto& it : s_items) {
			it->updateBuffer(index);
		}
	}

    void ShaderItem::flushData(void* data, uint32_t index) {
        memcpy(m_buffers[index].mapped, data, m_buffers[index].size);
    }
}