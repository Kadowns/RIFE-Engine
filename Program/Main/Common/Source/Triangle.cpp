#include <Triangle.h>

void Triangle::updateUniformBuffer(uint32_t currentImage) {
	
   
}

Triangle::Triangle(){

}


Triangle::~Triangle() {

}

void Triangle::init() {
	vkWrapper = APPLICATION->getVkWrapper();

    
    diffTex = TextureFactory::loadTexture("box.png");
    specTex = TextureFactory::loadTexture("box_specular.png");
    mat = MaterialFactory::surfaceMaterial(glm::vec4(1.0f), 16.0f, diffTex, specTex);
    cube = MeshFactory::createCube();
    plane = MeshFactory::createPlane(40, 40);

	gameObjects.push_back(new GameObject());
	gameObjects[0]->getTransform()->m_position = glm::vec3(0.0f, 0.2f, 6.0f);
    gameObjects[0]->addComponent(new Script::Movable());
	m_camera = (Camera*)gameObjects[0]->addComponent(new Camera());
	m_camera->updateProjection(
		55.0f,
		(float)APPLICATION->getWidth() / (float)APPLICATION->getHeight(),
		0.01f,
		100.0f
	);
    gameObjects[0]->setup();

    gameObjects.push_back(new GameObject());
    gameObjects[1]->addComponent(new MeshRenderer(plane, mat));
    gameObjects[1]->getTransform()->m_position = glm::vec3(0.0f, -4.0f, 0.0f);
    gameObjects[1]->setup();


	int newSize = gameObjects.size() + 4;
    for (int i = gameObjects.size(); i < newSize; i++) {
        gameObjects.push_back(new GameObject());
        gameObjects[i]->addComponent(new MeshRenderer(cube, mat));
        gameObjects[i]->addComponent(new Script::RotatingCube());
        gameObjects[i]->setup();
    }
	
    gameObjects[2]->getTransform()->m_position = glm::vec3(0.0f, 0.0f, -6.0f);
    gameObjects[3]->getTransform()->m_position = glm::vec3(-2.0f, -0.5f, -1.0f);
    gameObjects[4]->getTransform()->m_position = glm::vec3(2.0f, -0.5f, 1.0f);
    gameObjects[5]->getTransform()->m_position = glm::vec3(-2.0f, -0.5f, 1.0f);

}

void Triangle::awake() {
    for (int i = 0; i < gameObjects.size(); i++) {
        gameObjects[i]->awake();
    }
}

void Triangle::update() {
    for (int i = 0; i < gameObjects.size(); i++) {
        gameObjects[i]->update();
    }
    m_camera->update();
	//printf("\nFPS:%f", TIME->getFPS());
}

void Triangle::draw() {

	auto currentFrame = vkWrapper->getCurrentFrame();
	auto device = vkWrapper->getDevice();
	auto swapChain = vkWrapper->getSwapChain();
	auto inFlightFences = vkWrapper->getInFlightFences();
	auto imageAvailableSemaphores = vkWrapper->getImageAvailableSemaphores();
	auto renderFinishedSemaphores = vkWrapper->getRenderFinishedSemaphores();
	auto frameBufferResized = APPLICATION->framebufferResized();

	vkWaitForFences(device, 1, &(*inFlightFences)[*currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
	
	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(device, *swapChain, std::numeric_limits<uint64_t>::max(),
		(*imageAvailableSemaphores)[*currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		vkWrapper->recreateSwapChain();
		return;//PRECISA RETORNAR SE N FUDEU
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}
    
    VK_WRAPPER->updateUbos(imageIndex);
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { (*imageAvailableSemaphores)[*currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &(*vkWrapper->getCommandBuffers())[imageIndex];

	VkSemaphore signalSemaphores[] = { (*renderFinishedSemaphores)[*currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(device, 1, &(*inFlightFences)[*currentFrame]);
	if (vkQueueSubmit(vkWrapper->getGraphicsQueue(), 1, &submitInfo, (*inFlightFences)[*currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	VkSwapchainKHR swapChains[] = { *swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	result = vkQueuePresentKHR(vkWrapper->getPresentQueue(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || *frameBufferResized) {
		*frameBufferResized = false;
        vkWrapper->recreateSwapChain();
		windowResized(APPLICATION->getWidth(), APPLICATION->getHeight());
    }
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

	*currentFrame = (*currentFrame + 1) % vk::MAX_FRAMES_IN_FLIGHT;
}

void Triangle::deinit() {

    delete cube;
    delete plane;
    delete diffTex;
    delete specTex;
    
	for (int i = 0; i < gameObjects.size(); i++) {
		delete gameObjects[i];
	}
}

void Triangle::windowResized(const uint32_t& width, const uint32_t& height) {
    m_camera->updateProjection(m_camera->getFov(), (float)width / (float)height, m_camera->getNear(), m_camera->getFar());
}
