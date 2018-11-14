#pragma once

#include <VulkanData.h>

#include <Buffer.h>

namespace Rife::Graphics {

    class ShaderItem {

    public:

        ShaderItem() {}

        virtual size_t size() = 0;

        virtual ~ShaderItem() {};
        virtual void apply(VkDeviceMemory* memory, VkDeviceSize offset) = 0;
        virtual void setupBuffer() = 0;

        Buffer& getBuffer();

    protected:

        void flushData(void* data);

        Buffer m_buffer;
    };
}