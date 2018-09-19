#ifndef GLDEP
#define GLDEP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
//#include "vulkan/vulkan.h"
#endif // !GLDEP
#include "MathUtilities.h"
#include <iostream>
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

		VkInstance m_vkInstance;
		VkPhysicalDevice m_vkPhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_vkDevice;
		VkQueue m_vkGraphicsQueue, m_vkPresentQueue;
		VkDebugUtilsMessengerEXT m_vkCallback;
		VkSurfaceKHR m_vkSurface;
		VkSwapchainKHR m_vkSwapChain;

		std::vector<VkImage> m_vkSwapChainImages;
		VkFormat m_vkSwapChainImageFormat;
		VkExtent2D m_vkSwapChainExtent;

		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		std::vector<const char*> getRequiredExtensions();
		bool checkValidationLayerSupport();
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		int rateDeviceSuitability(VkPhysicalDevice device);
		void pickPhysicalDevice();
		void setupDebugCallback();
		void createVkInstance();
		void createSurface(GLFWwindow *window);
		void createLogicalDevice();
		void createSwapChain();

	public:
		Wrapper();
		void initializeVulkan(GLFWwindow *window, int windowW, int windowH);
		void terminateVulkan();

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);

	};
};