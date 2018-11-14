#pragma once

#include <VulkanInclude.h>

namespace Rife::Graphics {

    struct BufferInfo {
       
        VkBufferUsageFlags usageFlags;
        VkMemoryPropertyFlags memoryPropertyFlags;

    };

    struct Buffer {

        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;     
        VkDeviceSize size;
        void* mapped = nullptr;

        BufferInfo info;

        VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        void unmap();
        VkResult bind(VkDeviceSize offset = 0);

        void copyTo(void* data, VkDeviceSize size);

        void destroy();
    };


}