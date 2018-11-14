#include <Buffer.h>
#include <VulkanData.h>

#include <assert.h>

namespace Rife::Graphics {

    VkResult Buffer::map(VkDeviceSize size, VkDeviceSize offset) {
        return vkMapMemory(VK_DATA->getDevice(), memory, offset, size, 0, &mapped);
    }

    void Buffer::unmap() {

        if (mapped) {
            vkUnmapMemory(VK_DATA->getDevice(), memory);
            mapped = nullptr;
        }
    }

    VkResult Buffer::bind(VkDeviceSize offset) {
        return vkBindBufferMemory(VK_DATA->getDevice(), buffer, memory, offset);
    }

    void Buffer::copyTo(void* data, VkDeviceSize size) {
        assert(mapped);
        memcpy(mapped, data, size);
    }

    void Buffer::destroy() {
        if (buffer) {
            vkDestroyBuffer(VK_DATA->getDevice(), buffer, nullptr);
        }

        if (memory) {
            vkFreeMemory(VK_DATA->getDevice(), memory, nullptr);
        }
    }

}