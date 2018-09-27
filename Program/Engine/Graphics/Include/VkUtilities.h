#ifndef GLDEP
#define GLDEP

#define VERT_SHADER std::string("triVert.spv")
#define FRAG_SHADER std::string("triFrag.spv")

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif // !GLDEP
#include <Rm.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>

namespace vk {
    //Proxys pro debug--------------------
    extern VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pCallback);

    extern void DestroyDebugUtilsMessengerEXT(VkInstance instance,
        VkDebugUtilsMessengerEXT callback,
        const VkAllocationCallbacks* pAllocator);

    static std::vector<char> readFile(const std::string& filename);

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

    class Wrapper {

    private:
        uint32_t m_width, m_height;
        size_t m_currentFrame = 0;

		GLFWwindow* m_glfwWindow;

        VkInstance m_vkInstance;
        VkPhysicalDevice m_vkPhysicalDevice = VK_NULL_HANDLE;
        VkDevice m_vkDevice;
        VkQueue m_vkGraphicsQueue, m_vkPresentQueue;
        VkDebugUtilsMessengerEXT m_vkCallback;
        VkSurfaceKHR m_vkSurface;
        VkSwapchainKHR m_vkSwapChain;
        VkCommandPool m_vkCommandPool;

        std::vector<VkImage> m_vkSwapChainImages;
        std::vector<VkImageView> m_vkSwapChainImageViews;
        std::vector<VkFramebuffer> m_vkSwapChainFramebuffers;
        std::vector<VkCommandBuffer> m_vkCommandBuffers;
        std::vector<VkSemaphore> m_vkImageAvailableSemaphores;
        std::vector<VkSemaphore> m_vkRenderFinishedSemaphores;
        std::vector<VkFence> m_vkInFlightFences;
        VkFormat m_vkSwapChainImageFormat;
        VkExtent2D m_vkSwapChainExtent;
        VkRenderPass m_vkRenderPass;
        VkPipelineLayout m_vkPipelineLayout;
        VkPipeline m_vkGraphicsPipeline;
        

        //Helper Functions-------------------
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        VkShaderModule createShaderModule(const std::vector<char>& code);
        std::vector<const char*> getRequiredExtensions();	
        bool checkValidationLayerSupport();
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        int rateDeviceSuitability(VkPhysicalDevice device);
        //-----------------------------------

        //Initializer functions
        void pickPhysicalDevice();
        void setupDebugCallback();
        void createVkInstance();
        void createSurface();
        void createLogicalDevice();
        void createSwapChain();
        void createImageViews();
        void createRenderPass();
        void createGraphicsPipeline();
        void createFramebuffers();
        void createCommandPool();
        void createCommandBuffers();
        void createSyncObjects();
        //---------------------

		void cleanupSwapChain();

    public:

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);


        Wrapper(GLFWwindow *window);
        void initializeVulkan();
		void recreateSwapChain();
        void terminateVulkan();


        //Getters
        VkDevice* getDevice() { return &m_vkDevice; }
        std::vector<VkCommandBuffer>* getCommandBuffers() { return &m_vkCommandBuffers; }
        VkQueue* getGraphicsQueue() { return &m_vkGraphicsQueue; }
        VkQueue* getPresentQueue() { return &m_vkPresentQueue; }
        VkSwapchainKHR* getSwapChain() { return &m_vkSwapChain; }
        std::vector<VkSemaphore>* getImageAvailableSemaphores() { return &m_vkImageAvailableSemaphores; }
        std::vector<VkSemaphore>* getRenderFinishedSemaphores() { return &m_vkRenderFinishedSemaphores; }
        std::vector<VkFence>* getInFlightFences() { return &m_vkInFlightFences; }
        size_t* getCurrentFrame() { return &m_currentFrame; }

        //---------------------

    };
};