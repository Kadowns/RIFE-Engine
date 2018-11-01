#include <Triangle.h>

void Triangle::updateUniformBuffer(uint32_t currentImage) {
	
   
}

Triangle::Triangle(){

}


Triangle::~Triangle() {

}

void Triangle::init() {
	vkWrapper = APPLICATION->getVkWrapper();

    mat = MaterialFactory::surfaceMaterial(
		glm::vec4(1.0f),
		16.0f,
		DATABASE::getTexture("box"),
		DATABASE::getTexture("box_specular")
	);

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

    gameObjects.push_back(new GameObject());
    gameObjects[1]->addComponent(new MeshRenderer(DATABASE::getMesh("Plane"), mat));
    gameObjects[1]->getTransform()->m_position = glm::vec3(0.0f, -4.0f, 0.0f);



	int newSize = gameObjects.size() + 10;
    for (int i = gameObjects.size(); i < newSize; i++) {
        gameObjects.push_back(new GameObject());
        gameObjects[i]->addComponent(new MeshRenderer(DATABASE::getMesh(i % 2 == 0 ? "PolarSphere" : "NormalizedSphere"), mat));
        gameObjects[i]->addComponent(new Script::RotatingCube());
		gameObjects[i]->getTransform()->m_position = glm::vec3(4.0f - 2.0f * (i % 4), 0.0f, 10.0f - 2.0f * (i + 1));
    }

	for (size_t i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->setup();
	}

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

	uint32_t imageIndex;
	VkResult result = VK_BASE->prepareFrame(&imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		VK_BASE->recreateSwapChain();
		return;
	}
	std::vector<VkSemaphore> waitSemaphores, signalSemaphores;
	VK_BASE->submitFrame(imageIndex, waitSemaphores, signalSemaphores);
	
	result = VK_BASE->presentFrame(imageIndex, signalSemaphores);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || *APPLICATION->framebufferResized()) {
		*APPLICATION->framebufferResized() = false;
		VK_BASE->recreateSwapChain();
		windowResized(APPLICATION->getWidth(), APPLICATION->getHeight());
    }
}

void Triangle::deinit() {
	for (int i = 0; i < gameObjects.size(); i++) {
		delete gameObjects[i];
	}
}

void Triangle::windowResized(const uint32_t& width, const uint32_t& height) {
    m_camera->updateProjection(m_camera->getFov(), (float)width / (float)height, m_camera->getNear(), m_camera->getFar());
}
