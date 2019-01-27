#pragma once
#ifndef VK_BASE
#define VK_BASE Rife::Graphics::VulkanBase::getInstance()

#include <VulkanInclude.h>

#include <Event.h>

#include <Renderer.h>

#include <vector>

namespace Rife::Graphics {
    //Proxys pro debug--------------------
    extern VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pCallback);

    extern void DestroyDebugUtilsMessengerEXT(VkInstance instance,
        VkDebugUtilsMessengerEXT callback,
        const VkAllocationCallbacks* pAllocator);

    const int MAX_FRAMES_IN_FLIGHT = 2;

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_LUNARG_standard_validation"
    };

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    struct QueueFamilyIndices {
        int graphicsFamily = -1;
        int presentFamily = -1;
        bool isComplete() {
            return graphicsFamily >= 0 && presentFamily >= 0;
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class VulkanBase {

    private:

		static VulkanBase* s_instance;

        size_t m_currentFrame = 0;

        OnCleanupRenderer m_cleanupRendererEvent;
        OnCleanupPipeline m_cleanupPipelineEvent;
        OnRecreateRenderer m_recreateRendererEvent;
        OnRecreatePipeline m_recreatePipelineEvent;

        std::vector<Renderer*> m_renderers;
        std::vector<std::vector<VkCommandBuffer>*> m_secondaryCommandBuffers;

		GLFWwindow* m_glfwWindow;

        //Helper Functions-------------------
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		VkFormat findDepthFormat();
        std::vector<const char*> getRequiredExtensions();	
        bool checkValidationLayerSupport();
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        int rateDeviceSuitability(VkPhysicalDevice device);
        void prepareDrawCommandBuffer();
        void beginDrawCommands(uint32_t imageIndex);
        void recordDrawCommands(uint32_t imageIndex);
        //-----------------------------------

        //Initializer functions
        void pickPhysicalDevice();
        void setupDebugCallback();
        void createVulkanInstance();
        void createSurface();
        void createLogicalDevice();
        void createSwapchain();
        void createSwapChainImageViews();
        void createRenderPass();
        void createFramebuffers();
        void createCommandPool();
		void createDepthResources();
		
        void createCommandBuffers();
        void createSyncObjects();
        //---------------------

		void cleanupWindowResources();

    public:

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData
        );

		static VulkanBase* getInstance();

        VulkanBase(GLFWwindow *window);
        void initialSetup();
		void finalSetup();
		void windowResized();
        void terminateVulkan();

		VkResult prepareFrame(uint32_t* imageIndex);    
		void submitFrame(uint32_t& imageIndex, std::vector<VkSemaphore>& waitSemaphores, std::vector<VkSemaphore>& signalSemaphores);
		VkResult presentFrame(uint32_t& imageIndex, std::vector<VkSemaphore>& waitSemaphores);

		void bindCommandBuffer(std::vector<VkCommandBuffer>*);
		void bindRenderer(Rife::Graphics::Renderer*);

        size_t* getCurrentFrame() { return &m_currentFrame; }

        OnCleanupRenderer& onCleanupRenderer() { return m_cleanupRendererEvent; }
        OnCleanupPipeline& onCleanupPipeline() { return m_cleanupPipelineEvent; }
        OnRecreateRenderer& onRecreateRenderer() { return m_recreateRendererEvent; }
        OnRecreatePipeline& onRecreatePipeline() { return m_recreatePipelineEvent; }

        //---------------------

    };
};
#endif // !VK_BASE