#pragma once
#include <RifeCore.h>
#include <MaterialInstance.h>
#include <VulkanInclude.h>
#include <Scene.h>

namespace Rife::Graphics {

    class Renderer : public Base::Component {
    public:

        virtual ~Renderer();

        virtual void onInit() override;

        virtual void createCommandBuffers() = 0;
        virtual void freeCommandBuffers();

    protected:

        Renderer();

        void createDescriptorPool();
        void createDescriptorSets();


        OnCleanupRenderer::EventListener m_cleanupRendererCallback;
        OnRecreateRenderer::EventListener m_recreateRendererCallback;
        OnDraw::EventListener m_cmdDrawCallback;
       


        std::vector<VkCommandBuffer> m_commandBuffers;

        MaterialInstance m_material;

        std::vector<VkDescriptorSet> m_descriptorSets;
        VkDescriptorPool m_descriptorPool;

    private:

        void cmdDraw(const uint32_t& imageIndex);
    };
}
