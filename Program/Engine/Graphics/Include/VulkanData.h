#pragma once

#include <VulkanInclude.h>

#include <vector>

#define VK_DATA Rife::Graphics::VulkanData::getInstance()

namespace Rife::Graphics {

    class Renderer;

    class VulkanData final {

    public:

        VulkanData() { s_instance = this; }

        static VulkanData* getInstance() { return s_instance; }

        VkInstance& getVulkanInstance() { return m_instance; }
        VkPhysicalDevice& getPhysicalDevice() { return m_physicalDevice; }
        VkPhysicalDeviceProperties& getPhysicalDeviceProperties() { return m_physicalDeviceProperties; }
        VkPhysicalDeviceFeatures& getPhysicalDeviceFeatures() { return m_physicalDeviceFeatures; }
        VkFormatProperties& getFormatProperties() { return m_formatProperties; }
        VkDevice& getDevice() { return m_device; }
        VkQueue& getGraphicsQueue() { return m_graphicsQueue; }
        VkQueue& getPresentQueue() { return m_presentQueue; }

        VkDebugUtilsMessengerEXT& getDebugCallback() { return m_debugCallback; }

        VkSurfaceKHR& getSurface() { return m_surface; }
        VkSwapchainKHR& getSwapchain() { return m_swapchain; }
        std::vector<VkImage>& getSwapchainImages() { return m_swapchainImages; }
        std::vector<VkImageView>& getSwapchainImageViews() { return m_swapchainImageViews; }
        std::vector<VkFramebuffer>& getFramebuffers() { return m_swapchainFramebuffers; }
        VkFormat& getSwapchainImageFormat() { return m_swapchainImageFormat; }
        VkExtent2D& getExtent() { return m_swapChainExtent; }
        VkRenderPass& getRenderPass() { return m_renderPass; }

        VkCommandPool& getCommandPool() { return m_commandPool; }
        std::vector<VkCommandBuffer>& getPrimaryCommandBuffers() { return m_primaryCommandBuffers; }

        VkImage& getDepthImage() { return m_depthImage; }
        VkDeviceMemory& getDepthImageMemory() { return m_depthImageMemory; }
        VkImageView& getDepthImageView() { return m_depthImageView; }

        std::vector<VkSemaphore>& getImageAvailableSemaphores() { return m_imageAvailableSemaphores; }
        std::vector<VkSemaphore>& getRenderFinishedSemaphores() { return m_renderFinishedSemaphores; }
        std::vector<VkFence>& getInFlightFences() { return m_inFlightFences; }

        
    private:

        static VulkanData* s_instance;

        VkInstance m_instance;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkPhysicalDeviceProperties m_physicalDeviceProperties;
        VkPhysicalDeviceFeatures m_physicalDeviceFeatures;
        VkFormatProperties m_formatProperties;
        VkDevice m_device;
        VkQueue m_graphicsQueue, m_presentQueue;

        VkDebugUtilsMessengerEXT m_debugCallback;

        VkSurfaceKHR m_surface;
        VkSwapchainKHR m_swapchain;
        std::vector<VkImage> m_swapchainImages;
        std::vector<VkImageView> m_swapchainImageViews;
        std::vector<VkFramebuffer> m_swapchainFramebuffers;
        VkFormat m_swapchainImageFormat;
        VkExtent2D m_swapChainExtent;
        VkRenderPass m_renderPass;

        VkCommandPool m_commandPool;
        std::vector<VkCommandBuffer> m_primaryCommandBuffers;      

        VkImage m_depthImage;
        VkDeviceMemory m_depthImageMemory;
        VkImageView m_depthImageView;

        std::vector<VkSemaphore> m_imageAvailableSemaphores;
        std::vector<VkSemaphore> m_renderFinishedSemaphores;
        std::vector<VkFence> m_inFlightFences;

    };
}