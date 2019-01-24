#include <VulkanData.h>

namespace Rife::Graphics::Vulkan {

    VkInstance instance;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties physicalDeviceProperties;
    VkPhysicalDeviceFeatures physicalDeviceFeatures;
    VkFormatProperties formatProperties;
    VkDevice device;
    VkQueue graphicsQueue, presentQueue;

    VkDebugUtilsMessengerEXT debugCallback;

    VkSurfaceKHR surface;
    VkSwapchainKHR swapchain;
    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainImageViews;
    std::vector<VkFramebuffer> framebuffers;
    VkFormat swapchainImageFormat;
    VkExtent2D extent;
    VkRenderPass renderPass;

    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> primaryCommandBuffers;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

}