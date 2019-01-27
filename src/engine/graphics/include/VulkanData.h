#pragma once

#include <VulkanInclude.h>

#include <vector>

namespace Rife::Graphics::Vulkan {

    extern VkInstance instance;
    extern VkPhysicalDevice physicalDevice;
    extern VkPhysicalDeviceProperties physicalDeviceProperties;
    extern VkPhysicalDeviceFeatures physicalDeviceFeatures;
    extern VkFormatProperties formatProperties;
    extern VkDevice device;
    extern VkQueue graphicsQueue, presentQueue;

    extern VkDebugUtilsMessengerEXT debugCallback;

    extern VkSurfaceKHR surface;
    extern VkSwapchainKHR swapchain;
    extern std::vector<VkImage> swapchainImages;
    extern std::vector<VkImageView> swapchainImageViews;
    extern std::vector<VkFramebuffer> framebuffers;
    extern VkFormat swapchainImageFormat;
    extern VkExtent2D extent;
    extern VkRenderPass renderPass;

    extern VkCommandPool commandPool;
    extern std::vector<VkCommandBuffer> primaryCommandBuffers;

    extern VkImage depthImage;
    extern VkDeviceMemory depthImageMemory;
    extern VkImageView depthImageView;

    extern std::vector<VkSemaphore> imageAvailableSemaphores;
    extern std::vector<VkSemaphore> renderFinishedSemaphores;
    extern std::vector<VkFence> inFlightFences;

}