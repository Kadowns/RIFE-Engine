#include <Buffer.h>
#include <VulkanData.h>

#include <assert.h>

namespace Rife::Graphics {

    VkResult Buffer::map(VkDeviceSize size, VkDeviceSize offset) {
        return vkMapMemory(Vulkan::device, memory, offset, size, 0, &mapped);
    }

    void Buffer::unmap() {

        if (mapped) {
            vkUnmapMemory(Vulkan::device, memory);
            mapped = nullptr;
        }
    }

    VkResult Buffer::bind(VkDeviceSize offset) {
        return vkBindBufferMemory(Vulkan::device, buffer, memory, offset);
    }

    void Buffer::copyTo(void* data, VkDeviceSize size) {
        assert(mapped);
        memcpy(mapped, data, size);
    }

    void Buffer::destroy() {
        if (buffer) {
            vkDestroyBuffer(Vulkan::device, buffer, nullptr);
        }

        if (memory) {
            vkFreeMemory(Vulkan::device, memory, nullptr);
        }
    }

}