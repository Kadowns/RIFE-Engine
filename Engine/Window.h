#pragma once

//gl dependencies
#ifndef GLDEP
#define GLDEP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif // !GLDEP

#include "Scene.h"
#include "Keyboard.h"
#include "Time.h"

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>

#include <vector>
#include <chrono>
#include <thread>

#define WINDOW Window::getInstance()

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

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif
//----------------------------------------------------

class Window {
private :

	static Window *m_instance;	
	
	int m_width;
	int m_height;
	std::string m_title;
	bool m_initialized = false;

	Scene *m_scene;
	GLFWwindow *m_window;
	GLFWerrorfun m_errorCallback;
	VkInstance m_vkInstance;
	VkDebugUtilsMessengerEXT m_vkCallback;
	
	Window();

	void init();
	void initGlfw();
	void initVulkan();
	void setupDebugCallback();
	void createVkInstance();
	void loop();
	void terminate();

	
	std::vector<const char*> getRequiredExtensions();
	bool checkValidationLayerSupport();
		
public:

	static Window* getInstance();
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

	int getWidth();
	int getHeight();

	void define(Scene *scene, const std::string &title, int width, int height);
	Window(Scene *scene, const std::string &title, int width, int height);
	Window(Scene *scene, const std::string &title);
	Window(Scene *scene);
	virtual ~Window() = default;

    void show();	
};