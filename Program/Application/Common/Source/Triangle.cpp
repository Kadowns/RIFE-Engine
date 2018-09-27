#include <Triangle.h>

Triangle::Triangle(){

}


Triangle::~Triangle() {

}

void Triangle::init() {
	vkWrapper = APPLICATION->getVkWrapper();
}

void Triangle::update(float secs) {
	//printf("Time: %f, FPS : %f, Total frames: %i\n", secs, TIME->getFPS(), TIME->getTotalFrames());
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

}