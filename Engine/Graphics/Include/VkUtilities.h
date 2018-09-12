#ifndef GLDEP
#define GLDEP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
//#include "vulkan/vulkan.h"
#endif // !GLDEP
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

	class Wrapper {

	private:
		VkInstance m_vkInstance;
		VkPhysicalDevice m_vkPhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_vkDevice;
		VkQueue m_vkGraphicsQueue, m_vkPresentQueue;
		VkDebugUtilsMessengerEXT m_vkCallback;
		VkSurfaceKHR m_vkSurface;

		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

		std::vector<const char*> getRequiredExtensions();
		bool checkValidationLayerSupport();
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		int rateDeviceSuitability(VkPhysicalDevice device);
		void pickPhysicalDevice();
		void setupDebugCallback();
		void createVkInstance();
		void createSurface(GLFWwindow *window);
		void createLogicalDevice();

	public:
		Wrapper();
		void initializeVulkan(GLFWwindow *window);
		void terminateVulkan();

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);

	};
}