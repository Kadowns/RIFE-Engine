#include <VulkanBase.h>
#include <VulkanData.h>
#include <VulkanTools.h>

#include <RifeMath.h>

#include <set>
#include <map>
#include <array>

namespace Rife::Graphics {

	VulkanBase* VulkanBase::s_instance = nullptr;

	VulkanBase::VulkanBase(GLFWwindow *window) : m_glfwWindow(window) {
		s_instance = this;
	}

	VulkanBase* VulkanBase::getInstance() {
		return s_instance;
	}

	//Inicializa a vulkan
	void VulkanBase::initialSetup() {

		//Initial base
		createVulkanInstance();//Cria e configura a instancia
		setupDebugCallback();//caso seja no modo debug, cria e configura o debug callback
		createSurface();//Cria uma "superficie" na janela onde podemos desenhar
		pickPhysicalDevice();//escolhe a melhor GPU disponivel
		createLogicalDevice();//faz uns bagulho com queue e sei l� o que
		//---------------------------------
		//todo---configuravel VVVV
		createSwapchain();//Cria a swap chain, � uma queue que � usada pra mostrar as imagens na hora certa
		createSwapChainImageViews();//Cria o objeto que basicamente vai ser a imagem exibida
		createRenderPass();//Cria o render pass que eu n faço ideia do que é  
		createCommandPool();  
	}

	void VulkanBase::finalSetup() {


		createDepthResources();
		createFramebuffers();
		createCommandBuffers();
		createSyncObjects();
	}

	void VulkanBase::windowResized() {

		int width = 0, height = 0;
		while (width == 0 || height == 0) {
			glfwGetFramebufferSize(m_glfwWindow, &width, &height);
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(Vulkan::device);

		cleanupWindowResources();

		createSwapchain();
		createSwapChainImageViews();
		createRenderPass();
		createDepthResources();

        m_recreatePipelineEvent();

		createFramebuffers();    
		createCommandBuffers();
	}

	void VulkanBase::terminateVulkan() {

		cleanupWindowResources();

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(Vulkan::device, Vulkan::renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(Vulkan::device, Vulkan::imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(Vulkan::device, Vulkan::inFlightFences[i], nullptr);
		}

		vkDestroyCommandPool(Vulkan::device, Vulkan::commandPool, nullptr);

		vkDestroyDevice(Vulkan::device, nullptr);

		if (enableValidationLayers) {
			DestroyDebugUtilsMessengerEXT(Vulkan::instance, Vulkan::debugCallback, nullptr);
		}

		vkDestroySurfaceKHR(Vulkan::instance, Vulkan::surface, nullptr);
		vkDestroyInstance(Vulkan::instance, nullptr);
	}


	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanBase::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT * pCallbackData,
		void * pUserData) {

		std::cerr << "validation layer: " << pCallbackData->pMessage << "\n\n";

		return VK_FALSE;
	}




	QueueFamilyIndices VulkanBase::findQueueFamilies(VkPhysicalDevice device) {
		QueueFamilyIndices indices;
		
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, Vulkan::surface, &presentSupport);
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}
			if (queueFamily.queueCount > 0 && presentSupport) {
				indices.presentFamily = i;
			}

			if (indices.isComplete()) {
				break;
			}
			i++;
		}

		return indices;
	}

	SwapChainSupportDetails VulkanBase::querySwapChainSupport(VkPhysicalDevice device) {
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, Vulkan::surface, &details.capabilities);

		//Formats---------------------------------------
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, Vulkan::surface, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, Vulkan::surface, &formatCount, details.formats.data());
		}
		//----------------------------------------------


		//Presentation Modes----------------------------
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, Vulkan::surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, Vulkan::surface, &presentModeCount, details.presentModes.data());
		}
		//----------------------------------------------
		return details;
	}

	VkSurfaceFormatKHR VulkanBase::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
			return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
		}

		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR VulkanBase::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {
		VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
			else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
				bestMode = availablePresentMode;
			}
		}

		return  bestMode;
	}

	//Define o tamanho das imagens que v�o ser renderizadas (verificando se � possivel)
	VkExtent2D VulkanBase::chooseSwapExtent(const VkSurfaceCapabilitiesKHR & capabilities) {
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		else {

			int width, height;
			glfwGetFramebufferSize(m_glfwWindow, &width, &height);

			VkExtent2D actualExtent = { width, height };
			actualExtent.width = Rife::Math::clamp<uint32_t>(width,
				capabilities.minImageExtent.width, capabilities.maxImageExtent.width);

			actualExtent.height = Rife::Math::clamp<uint32_t>(height,
				capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	

	VkFormat VulkanBase::findDepthFormat() {
		return VulkanTools::findSupportedFormat(
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}

	std::vector<const char*> VulkanBase::getRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	bool VulkanBase::checkValidationLayerSupport() {
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

	bool VulkanBase::checkDeviceExtensionSupport(VkPhysicalDevice device) {
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		//lista as extens�es disponiveis
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		//requiredExtensions possuiu todas as extens�es necess�rias
		//nesse loop n�s pegamos as extens�es que possuimos e retiramos elas do requiredExtensions
		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		//caso todas as extens�es tenham sido apagadas do required extensions
		//significa que possuimos todas as que s�o necess�rias
		return requiredExtensions.empty();
	}

	int VulkanBase::rateDeviceSuitability(VkPhysicalDevice device) {

		///Pega informa��es da GPU
		//propriedades

		vkGetPhysicalDeviceProperties(device, &Vulkan::physicalDeviceProperties);
		//features
		VkPhysicalDeviceFeatures* deviceFeatures = &Vulkan::physicalDeviceFeatures;
		vkGetPhysicalDeviceFeatures(device, deviceFeatures);

		//se n�o tiver geometry shader, pula fora
		if (!deviceFeatures->geometryShader) {
			return 0;
		}

		if (!deviceFeatures->samplerAnisotropy) {
			return 0;
		}

        if (!deviceFeatures->textureCompressionASTC_LDR && !deviceFeatures->textureCompressionETC2 && !deviceFeatures->textureCompressionBC) {
            return 0;
        }

		//Verifica se possui suporte para os queue families
		auto families = findQueueFamilies(device);
		if (!families.isComplete())
			return 0;

		//verifica se possui as extens�es necess�rias
		bool extensionsSupported = checkDeviceExtensionSupport(device);
		if (!extensionsSupported)
			return 0;


		//Verifica se possuiu suporte de SwapChains (� basicamente uma queue das imagens que foram renderizadas e v�o ser printadas na tela)
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		if (swapChainSupport.formats.empty() || swapChainSupport.presentModes.empty()) {
			return 0;
		}

		//TODO------------fazer um sistema de scores mais eficiente
		int score = 0;

		if (Vulkan::physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			score += 999;
		}

		score += Vulkan::physicalDeviceProperties.limits.maxImageDimension2D;

		return score;
	}

    void VulkanBase::prepareDrawCommandBuffer() {

        Vulkan::primaryCommandBuffers.resize(Vulkan::swapchainImages.size());
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = Vulkan::commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = Vulkan::swapchainImages.size();

        if (vkAllocateCommandBuffers(Vulkan::device, &allocInfo, Vulkan::primaryCommandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void VulkanBase::beginDrawCommands(uint32_t imageIndex) {

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(Vulkan::primaryCommandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = Vulkan::renderPass;
        renderPassInfo.framebuffer = Vulkan::framebuffers[imageIndex];

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = Vulkan::extent;

        std::array<VkClearValue, 2> clearValues = {};

        clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(Vulkan::primaryCommandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
    }

    void VulkanBase::recordDrawCommands(uint32_t imageIndex) {

        beginDrawCommands(imageIndex);

        for (size_t cmdBufferIndex = 0; cmdBufferIndex < m_secondaryCommandBuffers.size(); cmdBufferIndex++) {
            vkCmdExecuteCommands(Vulkan::primaryCommandBuffers[imageIndex], 1, &m_secondaryCommandBuffers[cmdBufferIndex]->at(imageIndex));
        }

        vkCmdEndRenderPass(Vulkan::primaryCommandBuffers[imageIndex]);

        if (vkEndCommandBuffer(Vulkan::primaryCommandBuffers[imageIndex]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

	void VulkanBase::pickPhysicalDevice() {
		//verifica quantas placas de video suportam a vulkan no pc
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(Vulkan::instance, &deviceCount, nullptr);

		//se n�o houver nenhuma placa de video que suporta, para o programa
		if (deviceCount == 0) {
			throw std::runtime_error("failed to find GPUs with vulkan support!");
		}

		//lista todas as placas de video disponiveis
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(Vulkan::instance, &deviceCount, devices.data());

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
            Vulkan::physicalDevice = candidates.rbegin()->second;
		}
		else {
			//caso nenhuma preste
			throw std::runtime_error("no suitable GPU found!");
		}

	}

	void VulkanBase::setupDebugCallback() {
		if (!enableValidationLayers)
			return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

		//Qu�o importante a mensagem precisa ser para ser enviada
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		//Quais tipos de mensagem s�o enviadas
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		//ponteiro para a fun��o debugCallback (implementada nessa mesma classe)
		createInfo.pfnUserCallback = debugCallback;
		createInfo.pUserData = nullptr; // Optional

		if (CreateDebugUtilsMessengerEXT(Vulkan::instance, &createInfo, nullptr, &Vulkan::debugCallback) != VK_SUCCESS) {
			throw std::runtime_error("failed to set up debug callback!");
		}
	}

	void VulkanBase::createVulkanInstance() {


		if (enableValidationLayers) {
			if (!checkValidationLayerSupport())
				throw std::runtime_error("validation layers requested, but not available!");

		}

		//Informa��es pra vulkan fazer umas otimiza��es
		///Window Info-----------------------------------------------
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Vulkan Rife";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "RIFE";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;
		///---------------------------------------------------------------


		//Define umas baga�a global sei la tendi nada
		///Create Info-----------------------------------------------------
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;//guarda a struct l� de cima dentro dessa bagacinha

		auto extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		//passa pro struct as validation layers (serve pra testar se a gente ta fazendo merda no c�digo)
		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}
		///---------------------------------------------------------------

		if (vkCreateInstance(&createInfo, nullptr, &Vulkan::instance) != VK_SUCCESS) {
			throw std::runtime_error("failed to create vkInstance!");
		}
	}

	//Cria a superficie na janela 
	void VulkanBase::createSurface() {
		if (glfwCreateWindowSurface(Vulkan::instance, m_glfwWindow, nullptr, &Vulkan::surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}

	//Seta as queue e junta e linka elas no device (na vdd entendi quase nada)
	void VulkanBase::createLogicalDevice() {

		QueueFamilyIndices indices = findQueueFamilies(Vulkan::physicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };

		float queuePriority = 1.0f;
		for (int queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

        VkPhysicalDeviceFeatures deviceFeatures = Vulkan::physicalDeviceFeatures;
		//NOW IT IS TIME
		VkPhysicalDeviceFeatures enabledFeatures = {};
		enabledFeatures.fillModeNonSolid = VK_TRUE;
		enabledFeatures.samplerAnisotropy = VK_TRUE;
        if (deviceFeatures.textureCompressionBC) {
            enabledFeatures.textureCompressionBC = VK_TRUE;
        }
        else if (deviceFeatures.textureCompressionASTC_LDR) {
            enabledFeatures.textureCompressionASTC_LDR = VK_TRUE;
        }
        else if (deviceFeatures.textureCompressionETC2) {
            enabledFeatures.textureCompressionETC2 = VK_TRUE;
        }

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());;
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.pEnabledFeatures = &enabledFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(Vulkan::physicalDevice, &createInfo, nullptr, &Vulkan::device) != VK_SUCCESS) {
			throw std::runtime_error("failed to create logical device!");
		}

		vkGetDeviceQueue(Vulkan::device, indices.graphicsFamily, 0, &Vulkan::graphicsQueue);
		vkGetDeviceQueue(Vulkan::device, indices.presentFamily, 0, &Vulkan::presentQueue);
	}

	void VulkanBase::createSwapchain() {

		//Verifica o suporte de swapChains
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(Vulkan::physicalDevice);

		//Define que tipo de imagem vai ser gerada
		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);

		//Define qual vai ser a maneira que a swap chain vai se comportar (queue, ou simplesmente jogando tudo na tela)
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);

		//verifica se a swap chain consegue gerar imagens de qualque tamanho e, caso n�o consiga, define qual vai ser o tamanho permitido
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

        Vulkan::swapchainImageFormat = surfaceFormat.format;
        Vulkan::extent = extent;

		//caso maxImageCount seja 0, a queue da swapchain suporta qualquer quantidade de imagens
		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		//Informa��es da swapchain
		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = Vulkan::surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;//VK_IMAGE_USAGE_TRANSFER_DST_BIT para post process
		//-----------------------------

		QueueFamilyIndices indices = findQueueFamilies(Vulkan::physicalDevice);
		uint32_t queueFamilyIndices[] = { (uint32_t)indices.graphicsFamily, (uint32_t)indices.presentFamily };

		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0; // Optional
			createInfo.pQueueFamilyIndices = nullptr; // Optional
		}

		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;

		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;

		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(Vulkan::device, &createInfo, nullptr, &Vulkan::swapchain) != VK_SUCCESS) {
			throw std::runtime_error("failed to create swap chain!");
		}

		vkGetSwapchainImagesKHR(Vulkan::device, Vulkan::swapchain, &imageCount, nullptr);
        Vulkan::swapchainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(Vulkan::device, Vulkan::swapchain, &imageCount, Vulkan::swapchainImages.data());
	}

	void VulkanBase::createSwapChainImageViews() {
        Vulkan::swapchainImageViews.resize(Vulkan::swapchainImages.size());

        VkImageSubresourceRange subresourceRange = {};
        subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        subresourceRange.baseMipLevel = 0;
        subresourceRange.levelCount = 1;
        subresourceRange.baseArrayLayer = 0;
        subresourceRange.layerCount = 1;

		for (size_t i = 0; i < Vulkan::swapchainImageViews.size(); i++) {
            Vulkan::swapchainImageViews[i] = VulkanTools::createImageView(
                Vulkan::swapchainImages[i],
                Vulkan::swapchainImageFormat,
                VK_IMAGE_VIEW_TYPE_2D,
                subresourceRange
            );
		}
	}

	void VulkanBase::createRenderPass() {


		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = Vulkan::swapchainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentDescription depthAttachment = {};
		depthAttachment.format = findDepthFormat();
		depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthAttachmentRef = {};
		depthAttachmentRef.attachment = 1;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;


		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.pDepthStencilAttachment = &depthAttachmentRef;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

		std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(Vulkan::device, &renderPassInfo, nullptr, &Vulkan::renderPass) != VK_SUCCESS) {
			throw std::runtime_error("failed to create render pass!");
		}

	}

	void VulkanBase::createFramebuffers() {
        Vulkan::framebuffers.resize(Vulkan::swapchainImages.size());

		for (size_t i = 0; i < Vulkan::swapchainImages.size(); i++) {

			std::array<VkImageView, 2> attachments = {
                Vulkan::swapchainImageViews[i],
                Vulkan::depthImageView
			};

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = Vulkan::renderPass;
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = Vulkan::extent.width;
			framebufferInfo.height = Vulkan::extent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(Vulkan::device, &framebufferInfo, nullptr, &Vulkan::framebuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to create framebuffer!");
			}
		}
	}

	void VulkanBase::createCommandPool() {
		QueueFamilyIndices queueFamilyIndices = findQueueFamilies(Vulkan::physicalDevice);

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // permite resetar os commandbuffers a todo frame

		if (vkCreateCommandPool(Vulkan::device, &poolInfo, nullptr, &Vulkan::commandPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create command pool!");
		}
	}

	void VulkanBase::createDepthResources() {

		VkFormat depthFormat = findDepthFormat();

		VulkanTools::createImage(
            Vulkan::extent.width, Vulkan::extent.height, 1, 1,
			depthFormat,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            Vulkan::depthImage, Vulkan::depthImageMemory, 0
		);

        VkImageSubresourceRange subresourceRange = {};

        subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

        if (VulkanTools::hasStencilComponent(depthFormat)) {
            subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }

        subresourceRange.baseMipLevel = 0;
        subresourceRange.levelCount = 1;
        subresourceRange.baseArrayLayer = 0;
        subresourceRange.layerCount = 1;

        Vulkan::depthImageView = VulkanTools::createImageView(Vulkan::depthImage, depthFormat, VK_IMAGE_VIEW_TYPE_2D, subresourceRange);

		VulkanTools::transitionImageLayout(
            Vulkan::depthImage, depthFormat,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            subresourceRange
		);
	}

	VkResult VulkanBase::prepareFrame(uint32_t* imageIndex) {

		vkWaitForFences(Vulkan::device, 1, &Vulkan::inFlightFences[m_currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

		VkResult result = vkAcquireNextImageKHR(
            Vulkan::device,
            Vulkan::swapchain,
			std::numeric_limits<uint64_t>::max(),
            Vulkan::imageAvailableSemaphores[m_currentFrame],
			VK_NULL_HANDLE,
			imageIndex
		);

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		return result;
	}

	void VulkanBase::submitFrame(
		uint32_t& imageIndex,
		std::vector<VkSemaphore>& waitSemaphores,
		std::vector<VkSemaphore>& signalSemaphores
	) {
        ShaderItem::updateBuffers(imageIndex);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        recordDrawCommands(imageIndex);

		waitSemaphores = { Vulkan::imageAvailableSemaphores[m_currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
		submitInfo.pWaitSemaphores = waitSemaphores.data();
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &Vulkan::primaryCommandBuffers[imageIndex];

		signalSemaphores = { Vulkan::renderFinishedSemaphores[m_currentFrame] };

		submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
		submitInfo.pSignalSemaphores = signalSemaphores.data();

		vkResetFences(Vulkan::device, 1, &Vulkan::inFlightFences[m_currentFrame]);
		if (vkQueueSubmit(Vulkan::graphicsQueue, 1, &submitInfo, Vulkan::inFlightFences[m_currentFrame]) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}
	}

	VkResult VulkanBase::presentFrame(uint32_t& imageIndex, std::vector<VkSemaphore>& waitSemaphores) {
		
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
		presentInfo.pWaitSemaphores = waitSemaphores.data();
		
		VkSwapchainKHR swapChains[] = { Vulkan::swapchain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr; // Optional

		VkResult result = vkQueuePresentKHR(Vulkan::presentQueue, &presentInfo);

		m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;	

		return result;
	}

	void VulkanBase::bindRenderer(Rife::Graphics::Renderer* renderer) {
		m_renderers.push_back(renderer);
	}

	void VulkanBase::bindCommandBuffer(std::vector<VkCommandBuffer>* cmdBuffers) {
		m_secondaryCommandBuffers.push_back(cmdBuffers);
	}

	void VulkanBase::createCommandBuffers() {
        prepareDrawCommandBuffer();

        m_recreateRendererEvent();
		
	}

	void VulkanBase::createSyncObjects() {

        Vulkan::imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        Vulkan::renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        Vulkan::inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			if (vkCreateSemaphore(Vulkan::device, &semaphoreInfo, nullptr, &Vulkan::imageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(Vulkan::device, &semaphoreInfo, nullptr, &Vulkan::renderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(Vulkan::device, &fenceInfo, nullptr, &Vulkan::inFlightFences[i]) != VK_SUCCESS) {

				throw std::runtime_error("failed to create syncObjects for a frame!");
			}
		}
	}

	void VulkanBase::cleanupWindowResources() {


        m_cleanupRendererEvent();


		vkDestroyImageView(Vulkan::device, Vulkan::depthImageView, nullptr);
		vkDestroyImage(Vulkan::device, Vulkan::depthImage, nullptr);
		vkFreeMemory(Vulkan::device, Vulkan::depthImageMemory, nullptr);

		for (size_t i = 0; i < Vulkan::framebuffers.size(); i++) {
			vkDestroyFramebuffer(Vulkan::device, Vulkan::framebuffers[i], nullptr);
		}

		vkFreeCommandBuffers(
            Vulkan::device,
            Vulkan::commandPool,
            static_cast<uint32_t>(Vulkan::primaryCommandBuffers.size()),
            Vulkan::primaryCommandBuffers.data()
        );

        m_secondaryCommandBuffers.clear();

        m_cleanupPipelineEvent();

		vkDestroyRenderPass(Vulkan::device, Vulkan::renderPass, nullptr);

		for (size_t i = 0; i < Vulkan::swapchainImageViews.size(); i++) {
			vkDestroyImageView(Vulkan::device, Vulkan::swapchainImageViews[i], nullptr);
		}

		vkDestroySwapchainKHR(Vulkan::device, Vulkan::swapchain, nullptr);
	}


	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
		const VkAllocationCallbacks *pAllocator,
		VkDebugUtilsMessengerEXT *pCallback) {

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pCallback);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	//Proxys pro debug-----------------------------------
	void DestroyDebugUtilsMessengerEXT(VkInstance instance,
		VkDebugUtilsMessengerEXT callback,
		const VkAllocationCallbacks *pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, callback, pAllocator);
		}
	}
}
//---------------------------------------------------
