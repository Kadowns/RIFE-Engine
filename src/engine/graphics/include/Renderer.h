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
        virtual void freeCommandBuffers();

    protected:

        void createDescriptorPool();
        void createDescriptorSets();


        OnCleanupRenderer::EventListener m_cleanupRendererCallback;
        OnRecreateRenderer::EventListener m_recreateRendererCallback;


        std::vector<VkCommandBuffer> m_commandBuffers;

        MaterialInstance m_material;

        std::vector<VkDescriptorSet> m_descriptorSets;
        VkDescriptorPool m_descriptorPool;
    };
}
