#include <Triangle.h>

void Triangle::updateUniformBuffer(uint32_t currentImage) {
	
   
}

Triangle::Triangle(){

}


Triangle::~Triangle() {

}

void Triangle::init() {
	vkWrapper = APPLICATION->getVkWrapper();

	m_camera = new Camera(glm::vec3(0.0f, 4.0f, 8.0f), glm::vec3(0), glm::vec3(0.0f, 1.0f, 0.0f),
		45.0f, (float)APPLICATION->getWidth() / (float)APPLICATION->getHeight(), 0.01f, 100.0f);

    gameObjects.resize(7);
    auto mat = MaterialFactory::defaultMaterial("triVert.spv", "triFrag.spv");
    auto mesh = new Mesh(vertices, indices);
    for (int i = 0; i < gameObjects.size(); i++) {
        gameObjects[i] = new GameObject();
        gameObjects[i]->addComponent(new MeshRenderer(mesh, mat));
        gameObjects[i]->addComponent(new Script::RotatingCube());
        gameObjects[i]->setup();
    }

    gameObjects[0]->getTransform()->m_position = glm::vec3(2.0f, -0.5f, -1.0f);
    gameObjects[1]->getTransform()->m_position = m_camera->getPosition() + glm::vec3(0.0f, -1.5f, -6.0f);
    gameObjects[2]->getTransform()->m_position = glm::vec3(-2.0f, -0.5f, -1.0f);
    gameObjects[3]->getTransform()->m_position = glm::vec3(2.0f, -0.5f, 1.0f);
    gameObjects[4]->getTransform()->m_position = glm::vec3(-2.0f, -0.5f, 1.0f);
    gameObjects[5]->getTransform()->m_position = glm::vec3(4.0f, -0.5f, -4.0f);
    gameObjects[6]->getTransform()->m_position = glm::vec3(-4.0f, -0.5f, -4.0f);
    
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
}

void Triangle::draw() {

	auto currentFrame = vkWrapper->getCurrentFrame();
	auto device = vkWrapper->getDevice();
	auto swapChain = vkWrapper->getSwapChain();
	auto inFlightFences = vkWrapper->getInFlightFences();
	auto imageAvailableSemaphores = vkWrapper->getImageAvailableSemaphores();
	auto renderFinishedSemaphores = vkWrapper->getRenderFinishedSemaphores();
	auto frameBufferResized = APPLICATION->framebufferResized();

	vkWaitForFences(*device, 1, &(*inFlightFences)[*currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
	
	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(*device, *swapChain, std::numeric_limits<uint64_t>::max(),
		(*imageAvailableSemaphores)[*currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		vkWrapper->recreateSwapChain();
		return;//PRECISA RETORNAR SE N FUDEU
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

    glm::mat4 vp = CAMERA->getProjection() * CAMERA->getView();
    VK_WRAPPER->updateUbos(imageIndex, vp, TIME->time());
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

	vkResetFences(*device, 1, &(*inFlightFences)[*currentFrame]);
	if (vkQueueSubmit(*vkWrapper->getGraphicsQueue(), 1, &submitInfo, (*inFlightFences)[*currentFrame]) != VK_SUCCESS) {
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

	result = vkQueuePresentKHR(*vkWrapper->getPresentQueue(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || *frameBufferResized) {
		*frameBufferResized = false;
        vkWrapper->recreateSwapChain();
    }
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

	*currentFrame = (*currentFrame + 1) % vk::MAX_FRAMES_IN_FLIGHT;
}

void Triangle::deinit() {
	for (int i = 0; i < gameObjects.size(); i++) {
		delete gameObjects[i];
	}
}

void Triangle::windowResized(const uint32_t& width, const uint32_t& height) {
    m_camera->updateProjection(m_camera->getFov(), (float)width / (float)height, m_camera->getNear(), m_camera->getFar());
}
