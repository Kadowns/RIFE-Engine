#include <ShaderItem.h>
#include <VulkanTools.h>

namespace Rife::Graphics {

    Buffer& ShaderItem::getBuffer() {
        return m_buffer;
    }

    void ShaderItem::flushData(void * data) {
        memcpy(m_buffer.mapped, data, m_buffer.size);
    }
}