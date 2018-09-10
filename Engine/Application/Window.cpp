#include "Window.h"

//Proxys pro debug-----------------------------------
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, callback, pAllocator);
	}
}
//---------------------------------------------------

//--------------------------Singleton
Window* Window::m_instance = nullptr;

Window* Window::getInstance() {
	return m_instance != nullptr ? m_instance : (m_instance = new Window());
}
//----------------------------------

int Window::getWidth() {
	return m_width;
}

int Window::getHeight() {
	return m_height;
}

void Window::define(Scene * scene, const std::string & title, int width, int height) {
	m_scene = scene;
	m_title = title;
	m_width = width;
	m_height = height;
	m_initialized = false;
}

//---------------------------Constructors
Window::Window(Scene *scene, const std::string &title, int width, int height){
	m_scene = scene;
	m_title = title;
	m_width = width;
	m_height = height;
	m_instance = this;
}

Window::Window(Scene *scene, const std::string &title) : Window(scene, title, 800, 600) {}

Window::Window(Scene *scene): Window(scene, "Game") {}

Window::Window() {};
//---------------------------

//Inicializa a porra toda
void Window::init(){

	initGlfw();
	initVulkan();
	m_initialized = true;
}

//Inicializa a glfw
void Window::initGlfw() {
	if (!glfwInit())
		throw std::exception("Unable to initialize glfw");


	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);

	if (m_window == NULL)
		throw std::exception("Failed to create the GLFW Window");

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	glfwSwapInterval(1);
	glfwShowWindow(m_window);
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE);
	
}

//Inicializa a vulkan
void Window::initVulkan() {
	createVkInstance();
	setupDebugCallback();
}

void Window::setupDebugCallback() {
	if (!enableValidationLayers) 
		return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

	//Quão importante a mensagem precisa ser para ser enviada
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT 
		| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	//Quais tipos de mensagem são enviadas
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT 
		| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT 
		| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	//ponteiro para a função debugCallback (implementada nessa mesma classe)
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = nullptr; // Optional

	if (CreateDebugUtilsMessengerEXT(m_vkInstance, &createInfo, nullptr, &m_vkCallback) != VK_SUCCESS) {
		throw std::runtime_error("failed to set up debug callback!");
	}
}

void Window::createVkInstance() {

	if (enableValidationLayers) {
		if (!checkValidationLayerSupport())
			throw std::runtime_error("validation layers requested, but not available!");

	}
	
	//Informações pra vulkan fazer umas otimizações
	///Application Info-----------------------------------------------
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "RIFE";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;
	///---------------------------------------------------------------


	//Define umas bagaça global sei la tendi nada
	///Create Info-----------------------------------------------------
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;//guarda a struct lá de cima dentro dessa bagacinha

	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	//passa pro struct as validation layers (serve pra testar se a gente ta fazendo merda no código)
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}
	///---------------------------------------------------------------

	if (vkCreateInstance(&createInfo, nullptr, &m_vkInstance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create vkInstance!");
	}
}

void Window::pickPhysicalDevice() {

	//verifica quantas placas de video suportam a vulkan no pc
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, nullptr);

	//se não houver nenhuma placa de video que suporta, para o programa
	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with vulkan support!");
	}

	//lista todas as placas de video disponiveis
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, devices.data());

	//map usado pra rankear a melhor placa de video
	std::multimap<int, VkPhysicalDevice> candidates;

	for (const auto& device : devices) {
		//verifica a qualidade da placa de video
		int score = rateDeviceSuitability(device);

		//guarda a referencia da placa de video junto com seu score
		candidates.insert(std::make_pair(score, device));
	}
	
	//caso o score da melhor placa de video seja maior que zero, iremos utilizar ela
	if (candidates.rbegin()->first > 0) {
		m_vkPhysicalDevice = candidates.rbegin()->second;
	}
	else  {
		//caso nenhuma preste
		throw std::runtime_error("no suitable GPU found!");
	}

}


int Window::rateDeviceSuitability(VkPhysicalDevice device) {
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	if (!deviceFeatures.geometryShader) {
		return 0;
	}

	//TODO------------fazer um sistema de scores mais eficiente
	int score = 0;

	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		score += 999;
	}

	score += deviceProperties.limits.maxImageDimension2D;

	return score;
}

//loop principal, n tem segredo né
void Window::loop(){
	m_scene->init();
	do {
		TIME->earlyUpdate();
		
		double time = TIME->getLastFrameTime();
		m_scene->update(time);
		m_scene->draw();
		printf("Time: %f, FPS : %f, Total frames: %i\n", time, TIME->getFPS(), TIME->getTotalFrames());
		
		KEYBOARD->update();
		glfwSwapBuffers(m_window);
		glfwPollEvents();

		TIME->lateUpdate();

	} // se o maluco apertar os botão de sair, vc sai
	while (glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(m_window) == GLFW_FALSE);
	m_scene->deinit();
}

void Window::terminate() {

	if (enableValidationLayers) {
		DestroyDebugUtilsMessengerEXT(m_vkInstance, m_vkCallback, nullptr);
	}

	vkDestroyInstance(m_vkInstance, nullptr);
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

VKAPI_ATTR VkBool32 VKAPI_CALL Window::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

bool Window::checkValidationLayerSupport() {

	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

std::vector<const char*> Window::getRequiredExtensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}


//Função chamada pra inicializar a scene e manter o loop
void Window::show() {

	if (!m_initialized)
		init();

	loop();
	terminate();
}
