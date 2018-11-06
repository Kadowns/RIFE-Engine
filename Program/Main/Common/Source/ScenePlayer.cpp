#include <ScenePlayer.h>
#include <Serializer.h>

#include <RifeScripts.h>
#include <Movable.h>
#include <RotatingCube.h>

ScenePlayer::ScenePlayer(){

}


ScenePlayer::~ScenePlayer() {

}

void ScenePlayer::init() {


	gameObjects.push_back(new GameObject(new Transform()));
    auto t = gameObjects[0]->getComponent<Transform>();
    t->m_position = glm::vec3(0.0f, 0.2f, 6.0f);
    gameObjects[0]->addComponent(new Script::Movable());

	gameObjects[0]->addComponent(new Camera());
	CAMERA->updateProjection(
		55.0f,
		(float)APPLICATION->getWidth() / (float)APPLICATION->getHeight(),
		0.01f,
		1000.0f
	);
   // GLOBAL_LIGHTS->addPointLight((PointLight*)gameObjects[0]->addComponent(new PointLight(1.0, 0.09, 0.031)));
    
    

    Ubo::uMaterialProperties matProp = {};
    matProp.color = glm::vec4(1.0f);
    
    matProp.tiling = 32.0f;

    gameObjects.push_back(new GameObject(new Transform()));
    gameObjects[1]->addComponent(new MeshRenderer(DATABASE::getMesh("Plane"), MaterialInstance(DATABASE::getMaterial("Metal"), matProp)));
    t = gameObjects[1]->getComponent<Transform>();
    t->m_position = glm::vec3(0.0f, -4.0f, 0.0f);
    t->m_scale = glm::vec3(20.0f, 1.0f, 20.0f);

    matProp.tiling = 1.0f;
    matProp.specularPower = 128.0f;
    matProp.color = glm::vec4(0.9f, 0.5f, 0.5f, 1.0f);
    gameObjects.push_back(new GameObject(new Transform()));
    gameObjects[2]->addComponent(new MeshRenderer(DATABASE::getMesh("Ship"), MaterialInstance(DATABASE::getMaterial("Metal"), matProp)));
    gameObjects[2]->addComponent(new Script::RotatingCube());
    t = gameObjects[2]->getComponent<Transform>();
    t->m_position = glm::vec3(0.0f, 4.0f, 30.0f);
    t->m_rotation = glm::quat(glm::radians(glm::vec3(-90.0f, 0.0f, 0.0f)));
    //t->m_scale = glm::vec3(10.0f);

    matProp.tiling = 2.0f;
	int newSize = gameObjects.size() + 10;
    for (int i = gameObjects.size(); i < newSize; i++) {
        gameObjects.push_back(new GameObject(new Transform()));

        matProp.color = glm::vec4(Random::range(0.4f, 1.0f), Random::range(0.2f, 0.6f), Random::range(0.9f, 1.0f), 1.0f);
        matProp.specularPower = Random::range(1.0f, 64.0f);
        gameObjects[i]->addComponent(new MeshRenderer(DATABASE::getMesh("PolarSphere"), MaterialInstance(DATABASE::getMaterial("Metal"), matProp)));
        gameObjects[i]->addComponent(new Script::RotatingCube());
        t = gameObjects[i]->getComponent<Transform>();
        t->m_position = glm::vec3(40.0f - 20.0f * (i % 4), 0.0f, 10.0f - 2.0f * (i + 1));
    }

    gameObjects.push_back(new GameObject(new Transform()));
    int index = gameObjects.size() - 1;
    matProp.color = glm::vec4(1.0f);
    gameObjects[index]->addComponent(new MeshRenderer(DATABASE::getMesh("Cube"), MaterialInstance(DATABASE::getMaterial("Default"), matProp)));
    gameObjects[index]->addComponent(new PointLight(1.0, 0.03, 0.0014));
    gameObjects[index]->getComponent<Transform>()->m_position = glm::vec3(30, 15.0f, 5.0f);


	gameObjects[index]->addComponent(new DirectionalLight(glm::vec3(0, -1, 0), glm::vec3(1.0f), 0.0f));
        

    gameObjects.push_back(new GameObject(new Transform()));
    index = gameObjects.size() - 1;
    gameObjects[index]->getComponent<Transform>()->m_position = glm::vec3(-30, 15.0f, -5.0f);
    gameObjects[index]->addComponent(new MeshRenderer(DATABASE::getMesh("Cube"), MaterialInstance(DATABASE::getMaterial("Default"), matProp)));
    gameObjects[index]->addComponent(new PointLight(1.0, 0.03, 0.0014));

    
    Rife::Data::Serializer::saveScene("Main.scene", gameObjects);

	for (size_t i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->setup();
	}
}

void ScenePlayer::awake() {
    for (int i = 0; i < gameObjects.size(); i++) {
        gameObjects[i]->awake();
    }
}

void ScenePlayer::update() {
    for (int i = 0; i < gameObjects.size(); i++) {
        gameObjects[i]->update();
    }
    CAMERA->update();
    GLOBAL_LIGHTS->updateLightInfo();
}

void ScenePlayer::draw() {

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

void ScenePlayer::deinit() {
	for (int i = 0; i < gameObjects.size(); i++) {
		delete gameObjects[i];
	}
}

void ScenePlayer::windowResized(const uint32_t& width, const uint32_t& height) {
    CAMERA->updateProjection(CAMERA->getFov(), (float)width / (float)height, CAMERA->getNear(), CAMERA->getFar());
}
