#pragma once

#include <VulkanData.h>

namespace Rife::Graphics {

    class ShaderItem {
    public:

		virtual ~ShaderItem() {}
        virtual void apply(VkDeviceMemory* memory, VkDeviceSize offset) = 0;

    protected:

        void flushData(VkDeviceMemory* memory, VkDeviceSize range, VkDeviceSize offset, const void* dataValue) {

            void* dataPtr;
            vkMapMemory(VK_DATA->getDevice(), *memory, offset, range, 0, &dataPtr);
            memcpy(dataPtr, dataValue, range);
            vkUnmapMemory(VK_DATA->getDevice(), *memory);

        }
    };
}