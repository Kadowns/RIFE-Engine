#pragma once
#include <RifeCore.h>
#include <VulkanInclude.h>

namespace Rife::Graphics {

    class Renderer : public Base::Component {
    public:
        virtual void draw() = 0;
        virtual void createCommandBuffers() = 0;
        virtual void updateUbos(const uint32_t& imageIndex) = 0;
        virtual void freeCommandBuffers() = 0;
    protected:

		void createUniformBuffer(VkBuffer& buffer, VkDeviceMemory& memory, VkDeviceSize bufferSize);

        std::vector<VkCommandBuffer> m_commandBuffers;

    };
}
