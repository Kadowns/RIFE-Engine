#include <VulkanBase.h>

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
		createVkInstance();//Cria e configura a instancia
		setupDebugCallback();//caso seja no modo debug, cria e configura o debug callback
		createSurface();//Cria uma "superficie" na janela onde podemos desenhar
		pickPhysicalDevice();//escolhe a melhor GPU disponivel
		createLogicalDevice();//faz uns bagulho com queue e sei l� o que
	   // createDescriptorSetLayout();
		//---------------------------------
		//todo---configuravel VVVV
		createSwapChain();//Cria a swap chain, � uma queue que � usada pra mostrar as imagens na hora certa
		createSwapChainImageViews();//Cria o objeto que basicamente vai ser a imagem exibida
		createRenderPass();//Cria o render pass que eu n faço ideia do que é  
		createCommandPool();
		//createGraphicsPipeline();//CRIA A FUCKING GRAPHICS PIPELINE   
	}

	void VulkanBase::finalSetup() {


		createDepthResources();
		createFramebuffers();//Frame buffers 
		//-----------------------
		for (int i = 0; i < m_renderers.size(); i++) {
			m_renderers[i]->createCommandBuffers();
		}
		createCommandBuffer();
		createSyncObjects();
	}

	void VulkanBase::recreateSwapChain() {

		int width = 0, height = 0;
		while (width == 0 || height == 0) {
			glfwGetFramebufferSize(m_glfwWindow, &width, &height);
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(m_vkDevice);

		for (int i = 0; i < m_renderers.size(); i++) {
			m_renderers[i]->freeCommandBuffers();
		}

		m_secondaryCommandBuffers.clear();

		cleanupSwapChain();

		createSwapChain();
		createSwapChainImageViews();
		createRenderPass();


		createDepthResources();
		createFramebuffers();
		for (int i = 0; i < m_renderers.size(); i++) {
			m_renderers[i]->createCommandBuffers();
		}
		createCommandBuffer();
	}

	void VulkanBase::terminateVulkan() {

		cleanupSwapChain();

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(m_vkDevice, m_vkRenderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(m_vkDevice, m_vkImageAvailableSemaphores[i], nullptr);
			vkDestroyFence(m_vkDevice, m_vkInFlightFences[i], nullptr);
		}

		vkDestroyCommandPool(m_vkDevice, m_vkCommandPool, nullptr);

		vkDestroyDevice(m_vkDevice, nullptr);

		if (enableValidationLayers) {
			DestroyDebugUtilsMessengerEXT(m_vkInstance, m_vkCallback, nullptr);
		}

		vkDestroySurfaceKHR(m_vkInstance, m_vkSurface, nullptr);
		vkDestroyInstance(m_vkInstance, nullptr);
	}


	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanBase::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT * pCallbackData,
		void * pUserData) {

		std::cerr << "\nvalidation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}



	VkCommandBuffer VulkanBase::beginSingleTimeCommands() {

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_vkCommandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(m_vkDevice, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
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
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_vkSurface, &presentSupport);
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

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_vkSurface, &details.capabilities);

		//Formats---------------------------------------
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_vkSurface, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_vkSurface, &formatCount, details.formats.data());
		}
		//----------------------------------------------


		//Presentation Modes----------------------------
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_vkSurface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_vkSurface, &presentModeCount, details.presentModes.data());
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
			m_width = static_cast<uint32_t>(width);
			m_height = static_cast<uint32_t>(height);

			VkExtent2D actualExtent = { m_width, m_height };
			actualExtent.width = Rife::Math::clamp<uint32_t>(m_width,
				capabilities.minImageExtent.width, capabilities.maxImageExtent.width);

			actualExtent.height = Rife::Math::clamp<uint32_t>(m_height,
				capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	VkImageView VulkanBase::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {

		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = aspectFlags;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VkImageView imageView;
		if (vkCreateImageView(m_vkDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture image view!");
		}

		return imageView;
	}

	VkFormat VulkanBase::findDepthFormat() {
		return findSupportedFormat(
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}

	VkFormat VulkanBase::findSupportedFormat(const std::vector<VkFormat>& candidates,
		VkImageTiling tiling, VkFormatFeatureFlags features) {

		for (VkFormat format : candidates) {
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(m_vkPhysicalDevice, format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
				return format;
			}
		}
		throw std::runtime_error("failed to find supported format!");
	}

	VkShaderModule VulkanBase::createShaderModule(const std::vector<char>& code) {

		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		//Cast de char* para uint32_t* 
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(m_vkDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shader module!");
		}
		return shaderModule;
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

	uint32_t VulkanBase::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {

		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(m_vkPhysicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}

	bool VulkanBase::hasStencilComponent(VkFormat format) {
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
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

		vkGetPhysicalDeviceProperties(device, &m_physicalDeviceProperties);

		//features
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		//se n�o tiver geometry shader, pula fora
		if (!deviceFeatures.geometryShader) {
			return 0;
		}

		if (!deviceFeatures.samplerAnisotropy) {
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

		if (m_physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			score += 999;
		}

		score += m_physicalDeviceProperties.limits.maxImageDimension2D;

		return score;
	}

	void VulkanBase::endSingleTimeCommands(VkCommandBuffer commandBuffer) {

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(m_vkGraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(m_vkGraphicsQueue);

		vkFreeCommandBuffers(m_vkDevice, m_vkCommandPool, 1, &commandBuffer);
	}

	void VulkanBase::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {

		VkCommandBuffer commandBuffer = beginSingleTimeCommands();

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;

		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		barrier.image = image;

		if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

			if (hasStencilComponent(format)) {
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
		}
		else {
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}

		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else {
			throw std::invalid_argument("unsupported layout transition!");
		}

		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		endSingleTimeCommands(commandBuffer);
	}

	void VulkanBase::pickPhysicalDevice() {
		//verifica quantas placas de video suportam a vulkan no pc
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, nullptr);

		//se n�o houver nenhuma placa de video que suporta, para o programa
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

		if (CreateDebugUtilsMessengerEXT(m_vkInstance, &createInfo, nullptr, &m_vkCallback) != VK_SUCCESS) {
			throw std::runtime_error("failed to set up debug callback!");
		}
	}

	void VulkanBase::createVkInstance() {


		if (enableValidationLayers) {
			if (!checkValidationLayerSupport())
				throw std::runtime_error("validation layers requested, but not available!");

		}

		//Informa��es pra vulkan fazer umas otimiza��es
		///Application Info-----------------------------------------------
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Vulkan triangle";
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

		if (vkCreateInstance(&createInfo, nullptr, &m_vkInstance) != VK_SUCCESS) {
			throw std::runtime_error("failed to create vkInstance!");
		}
	}

	//Cria a superficie na janela 
	void VulkanBase::createSurface() {
		if (glfwCreateWindowSurface(m_vkInstance, m_glfwWindow, nullptr, &m_vkSurface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}

	//Seta as queue e junta e linka elas no device (na vdd entendi quase nada)
	void VulkanBase::createLogicalDevice() {

		QueueFamilyIndices indices = findQueueFamilies(m_vkPhysicalDevice);

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

		//soon
		VkPhysicalDeviceFeatures deviceFeatures = {};
		deviceFeatures.fillModeNonSolid = VK_TRUE;
		deviceFeatures.samplerAnisotropy = VK_TRUE;

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());;
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(m_vkPhysicalDevice, &createInfo, nullptr, &m_vkDevice) != VK_SUCCESS) {
			throw std::runtime_error("failed to create logical device!");
		}

		vkGetDeviceQueue(m_vkDevice, indices.graphicsFamily, 0, &m_vkGraphicsQueue);
		vkGetDeviceQueue(m_vkDevice, indices.presentFamily, 0, &m_vkPresentQueue);
	}

	void VulkanBase::createSwapChain() {

		//Verifica o suporte de swapChains
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_vkPhysicalDevice);

		//Define que tipo de imagem vai ser gerada
		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);

		//Define qual vai ser a maneira que a swap chain vai se comportar (queue, ou simplesmente jogando tudo na tela)
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);

		//verifica se a swap chain consegue gerar imagens de qualque tamanho e, caso n�o consiga, define qual vai ser o tamanho permitido
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

		m_vkSwapChainImageFormat = surfaceFormat.format;
		m_vkSwapChainExtent = extent;

		//caso maxImageCount seja 0, a queue da swapchain suporta qualquer quantidade de imagens
		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		//Informa��es da swapchain
		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_vkSurface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;//VK_IMAGE_USAGE_TRANSFER_DST_BIT para post process
		//-----------------------------

		QueueFamilyIndices indices = findQueueFamilies(m_vkPhysicalDevice);
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

		if (vkCreateSwapchainKHR(m_vkDevice, &createInfo, nullptr, &m_vkSwapChain) != VK_SUCCESS) {
			throw std::runtime_error("failed to create swap chain!");
		}

		vkGetSwapchainImagesKHR(m_vkDevice, m_vkSwapChain, &imageCount, nullptr);
		m_vkSwapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(m_vkDevice, m_vkSwapChain, &imageCount, m_vkSwapChainImages.data());


	}

	void VulkanBase::createSwapChainImageViews() {
		m_vkSwapChainImageViews.resize(m_vkSwapChainImages.size());

		for (size_t i = 0; i < m_vkSwapChainImages.size(); i++) {
			m_vkSwapChainImageViews[i] = createImageView(m_vkSwapChainImages[i], m_vkSwapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
		}
	}

	void VulkanBase::createRenderPass() {


		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = m_vkSwapChainImageFormat;
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

		if (vkCreateRenderPass(m_vkDevice, &renderPassInfo, nullptr, &m_vkRenderPass) != VK_SUCCESS) {
			throw std::runtime_error("failed to create render pass!");
		}

	}

	void VulkanBase::createFramebuffers() {
		m_vkSwapChainFramebuffers.resize(m_vkSwapChainImages.size());

		for (size_t i = 0; i < m_vkSwapChainImageViews.size(); i++) {

			std::array<VkImageView, 2> attachments = {
				m_vkSwapChainImageViews[i],
				m_vkDepthImageView
			};

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = m_vkRenderPass;
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = m_vkSwapChainExtent.width;
			framebufferInfo.height = m_vkSwapChainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(m_vkDevice, &framebufferInfo, nullptr, &m_vkSwapChainFramebuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to create framebuffer!");
			}
		}
	}

	void VulkanBase::createCommandPool() {
		QueueFamilyIndices queueFamilyIndices = findQueueFamilies(m_vkPhysicalDevice);

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
		poolInfo.flags = 0; // Optional

		if (vkCreateCommandPool(m_vkDevice, &poolInfo, nullptr, &m_vkCommandPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create command pool!");
		}
	}

	void VulkanBase::createDepthResources() {

		VkFormat depthFormat = findDepthFormat();

		createImage(
			m_vkSwapChainExtent.width, m_vkSwapChainExtent.height,
			depthFormat,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_vkDepthImage, m_vkDepthImageMemory
		);

		m_vkDepthImageView = createImageView(m_vkDepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

		transitionImageLayout(
			m_vkDepthImage, depthFormat,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
		);
	}

	VkResult VulkanBase::prepareFrame(uint32_t* imageIndex) {

		vkWaitForFences(m_vkDevice, 1, &m_vkInFlightFences[m_currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

		VkResult result = vkAcquireNextImageKHR(
			m_vkDevice,
			m_vkSwapChain,
			std::numeric_limits<uint64_t>::max(),
			m_vkImageAvailableSemaphores[m_currentFrame],
			VK_NULL_HANDLE,
			imageIndex
		);

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		return result;
	}

	void VulkanBase::submitUniformBuffersInfo(uint32_t imageIndex) {
		for (int i = 0; i < m_renderers.size(); i++) {
			m_renderers[i]->submitUniformBuffersInfo(imageIndex);
		}
	}

	void VulkanBase::submitFrame(
		uint32_t& imageIndex,
		std::vector<VkSemaphore>& waitSemaphores,
		std::vector<VkSemaphore>& signalSemaphores
	) {
		submitUniformBuffersInfo(imageIndex);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		waitSemaphores = { m_vkImageAvailableSemaphores[m_currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
		submitInfo.pWaitSemaphores = waitSemaphores.data();
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_primaryCommandBuffers[imageIndex];

		signalSemaphores = { m_vkRenderFinishedSemaphores[m_currentFrame] };

		submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
		submitInfo.pSignalSemaphores = signalSemaphores.data();

		vkResetFences(m_vkDevice, 1, &m_vkInFlightFences[m_currentFrame]);
		if (vkQueueSubmit(m_vkGraphicsQueue, 1, &submitInfo, m_vkInFlightFences[m_currentFrame]) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}
	}

	VkResult VulkanBase::presentFrame(uint32_t& imageIndex, std::vector<VkSemaphore>& waitSemaphores) {
		
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
		presentInfo.pWaitSemaphores = waitSemaphores.data();
		
		VkSwapchainKHR swapChains[] = { m_vkSwapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr; // Optional

		VkResult result = vkQueuePresentKHR(m_vkPresentQueue, &presentInfo);

		m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}

		return result;
	}

	void VulkanBase::bindRenderer(Rife::Graphics::Renderer* renderer) {
		m_renderers.push_back(renderer);
	}

	void VulkanBase::bindCommandBuffer(std::vector<VkCommandBuffer>* cmdBuffers) {
		m_secondaryCommandBuffers.push_back(cmdBuffers);
	}

	void VulkanBase::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(m_vkDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(m_vkDevice, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

		//TODO --- implementar solução top de alocação de memória (ver vulkan tutorial)
		if (vkAllocateMemory(m_vkDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate buffer memory!");
		}

		vkBindBufferMemory(m_vkDevice, buffer, bufferMemory, 0);
	}

	void VulkanBase::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDeviceSize offset) {

		VkCommandBuffer commandBuffer = beginSingleTimeCommands();

		VkBufferCopy copyRegion = {};
		copyRegion.srcOffset = 0; // Optional
		copyRegion.dstOffset = offset; // Optional
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		endSingleTimeCommands(commandBuffer);
	}

	void VulkanBase::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {

		VkImageCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateImage(m_vkDevice, &imageInfo, nullptr, &image) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(m_vkDevice, image, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(m_vkDevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate image memory!");
		}

		vkBindImageMemory(m_vkDevice, image, imageMemory, 0);
	}

	void VulkanBase::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {

		VkCommandBuffer commandBuffer = beginSingleTimeCommands();

		VkBufferImageCopy region = {};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = {
			width,
			height,
			1
		};

		vkCmdCopyBufferToImage(
			commandBuffer,
			buffer,
			image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&region
		);

		endSingleTimeCommands(commandBuffer);

	}

	void VulkanBase::createCommandBuffer() {
		m_primaryCommandBuffers.resize(m_vkSwapChainFramebuffers.size());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_vkCommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)m_primaryCommandBuffers.size();

		if (vkAllocateCommandBuffers(m_vkDevice, &allocInfo, m_primaryCommandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers!");
		}

		for (size_t i = 0; i < m_primaryCommandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
			beginInfo.pInheritanceInfo = nullptr; // Optional

			if (vkBeginCommandBuffer(m_primaryCommandBuffers[i], &beginInfo) != VK_SUCCESS) {
				throw std::runtime_error("failed to begin recording command buffer!");
			}

			VkRenderPassBeginInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = m_vkRenderPass;
			renderPassInfo.framebuffer = m_vkSwapChainFramebuffers[i];

			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = m_vkSwapChainExtent;

			std::array<VkClearValue, 2> clearValues = {};

			clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
			clearValues[1].depthStencil = { 1.0f, 0 };

			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(m_primaryCommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);

			for (size_t cmdBufferIndex = 0; cmdBufferIndex < m_secondaryCommandBuffers.size(); cmdBufferIndex++) {
				vkCmdExecuteCommands(m_primaryCommandBuffers[i], 1, &m_secondaryCommandBuffers[cmdBufferIndex]->at(i));
			}

			vkCmdEndRenderPass(m_primaryCommandBuffers[i]);

			if (vkEndCommandBuffer(m_primaryCommandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to record command buffer!");
			}
		}
	}

	void VulkanBase::createSyncObjects() {

		m_vkImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_vkRenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_vkInFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			if (vkCreateSemaphore(m_vkDevice, &semaphoreInfo, nullptr, &m_vkImageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(m_vkDevice, &semaphoreInfo, nullptr, &m_vkRenderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(m_vkDevice, &fenceInfo, nullptr, &m_vkInFlightFences[i]) != VK_SUCCESS) {

				throw std::runtime_error("failed to create syncObjects for a frame!");
			}
		}
	}

	void VulkanBase::cleanupSwapChain() {

		vkDestroyImageView(m_vkDevice, m_vkDepthImageView, nullptr);
		vkDestroyImage(m_vkDevice, m_vkDepthImage, nullptr);
		vkFreeMemory(m_vkDevice, m_vkDepthImageMemory, nullptr);

		for (size_t i = 0; i < m_vkSwapChainFramebuffers.size(); i++) {
			vkDestroyFramebuffer(m_vkDevice, m_vkSwapChainFramebuffers[i], nullptr);
		}


		vkFreeCommandBuffers(m_vkDevice, m_vkCommandPool, static_cast<uint32_t>(m_primaryCommandBuffers.size()), m_primaryCommandBuffers.data());

		/* vkDestroyPipeline(m_vkDevice, m_vkGraphicsPipeline, nullptr);
		 vkDestroyPipelineLayout(m_vkDevice, m_vkPipelineLayout, nullptr);*/
		vkDestroyRenderPass(m_vkDevice, m_vkRenderPass, nullptr);

		for (size_t i = 0; i < m_vkSwapChainImageViews.size(); i++) {
			vkDestroyImageView(m_vkDevice, m_vkSwapChainImageViews[i], nullptr);
		}

		vkDestroySwapchainKHR(m_vkDevice, m_vkSwapChain, nullptr);
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
