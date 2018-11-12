#pragma once
#include <RifeCore.h>
#include <MaterialInstance.h>
#include <VulkanInclude.h>

namespace Rife::Graphics {

    class Renderer : public Base::Component {
    public:

        virtual ~Renderer();

        virtual void setup();

        virtual void createCommandBuffers() = 0;
        virtual void submitUniformBuffersInfo(const uint32_t& imageIndex) = 0;
        virtual void freeCommandBuffers();



    protected:

		void createUniformBuffer(VkBuffer& buffer, VkDeviceMemory& memory, VkDeviceSize bufferSize);
        void createDescriptorPool();
        void createDescriptorSets();

        std::vector<VkCommandBuffer> m_commandBuffers;

        MaterialInstance m_material;

        std::vector<VkBuffer> m_uniformBuffers;
        std::vector<VkDeviceMemory> m_uniformBuffersMemory;
        std::vector<VkDescriptorSet> m_descriptorSets;
        VkDescriptorPool m_descriptorPool;
    };
}
