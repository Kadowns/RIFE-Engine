#pragma once

#include <GraphicsDependencys.h>
#include <Vertex.h>
#include <UniformBufferObject.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <set>
#include <map>

#define VERT_SHADER std::string("triVert.spv")
#define FRAG_SHADER std::string("triFrag.spv")

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

    const std::vector<gph::Vertex> vertices = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
    };

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

		VkBuffer m_vkVertexBuffer;
		VkDeviceMemory m_vkVertexBufferMemory;

		VkBuffer m_vkIndexBuffer;
		VkDeviceMemory m_vkIndexBufferMemory;

		std::vector<VkBuffer> m_vkUniformBuffers;
		std::vector<VkDeviceMemory> m_vkUniformBuffersMemory;

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
		VkDescriptorSetLayout m_vkDescriptorSetLayout;
		VkDescriptorPool m_vkDescriptorPool;
		std::vector<VkDescriptorSet> m_vkDescriptorSets;
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
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        bool checkValidationLayerSupport();
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        int rateDeviceSuitability(VkPhysicalDevice device);
		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
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
		void createDescriptorSetLayout();
        void createGraphicsPipeline();
        void createFramebuffers();
        void createCommandPool();
		void createVertexBuffer();
		void createIndexBuffer();
		void createUniformBuffer();
		void createDescriptorPool();
		void createDescriptorSets();
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
		std::vector<VkBuffer>* getUniformBuffers() { return &m_vkUniformBuffers; }
		std::vector<VkDeviceMemory>* getUniformBufferMemory() { return &m_vkUniformBuffersMemory; }
		VkExtent2D* getVkExtent() { return &m_vkSwapChainExtent; }
        size_t* getCurrentFrame() { return &m_currentFrame; }


        //---------------------

    };
};