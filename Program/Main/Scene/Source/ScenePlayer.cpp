#include <ScenePlayer.h>
#include <Serializer.h>

#include <RifeAssets.h>

using namespace Scripts;

ScenePlayer::ScenePlayer(){

}


ScenePlayer::~ScenePlayer() {

}

void ScenePlayer::init() {


	gameObjects.push_back(new GameObject(new Transform()));
    gameObjects[0]->getComponent<Transform>()->m_position = glm::vec3(0.0f, 0.2f, 6.0f);
    gameObjects[0]->addComponent(new Movable());
	gameObjects[0]->addComponent(new Camera());
    
    
    Ubo::MaterialProperties matProp = {};
    matProp.color = glm::vec4(1.0f);
	matProp.reflectionPower = 0;
	matProp.specularPower = 0;
    matProp.tiling = 4.0f;
    
    TerrainCreateInfo terrainInfo = {};
    terrainInfo.extent.x = 128;
    terrainInfo.extent.z = 128;
    terrainInfo.seed = 0;
    terrainInfo.octaves = 8;
    terrainInfo.scale = 16.0f;
    terrainInfo.lacunarity = 2.5f;
    terrainInfo.persistence = 0.25f;

    terrainInfo.weights[0].min = 0.0f;
    terrainInfo.weights[0].max = 0.35f;

    terrainInfo.weights[1].min = 0.3f;
    terrainInfo.weights[1].max = 0.7f;

    terrainInfo.weights[2].min = 0.45f;
    terrainInfo.weights[2].max = 0.9f;

    terrainInfo.weights[3].min = 0.85f;
    terrainInfo.weights[3].max = 1.0f;

    gameObjects.push_back(new GameObject(new Transform()));
    gameObjects[1]->addComponent(new MeshRenderer(
        DATABASE::insertMesh(TerrainGenerator::generateTerrain(terrainInfo), "Terrain"),
        MaterialInstance(DATABASE::getMaterial("Terrain"), matProp)));
    auto t = gameObjects[1]->getComponent<Transform>();
    t->m_position = glm::vec3(0.0f, -450.0f, 0.0f);
    t->m_scale = glm::vec3(50.0f, 480.0f, 50.0f);

    matProp.tiling = 1.0f;
	int newSize = gameObjects.size() + 10;
    for (int i = gameObjects.size(); i < newSize; i++) {
        gameObjects.push_back(new GameObject(new Transform()));

        matProp.color = glm::vec4(Random::range(0.5f, 1.0f), Random::range(0.5f, 1.0f), Random::range(0.5f, 1.0f), 1.0f);
        matProp.specularPower = Random::range(1.0f, 256.0f);
        matProp.reflectionPower = Random::range(0.5f, 1.0f);

        gameObjects[i]->addComponent(new MeshRenderer(DATABASE::getMesh("PolarSphere"), MaterialInstance(DATABASE::getMaterial("Metal"), matProp)));
        gameObjects[i]->addComponent(new RotatingCube());
        t = gameObjects[i]->getComponent<Transform>();
        t->m_position = glm::vec3(40.0f - 20.0f * (i % 4), 0.0f, 10.0f - 2.0f * (i + 1));
    }

    gameObjects.push_back(new GameObject(new Transform()));
    int index = gameObjects.size() - 1;
    matProp.color = glm::vec4(1.0f);
    matProp.reflectionPower = 0;
    matProp.specularPower = 0;
    gameObjects[index]->addComponent(new MeshRenderer(DATABASE::getMesh("Cube"), MaterialInstance(DATABASE::getMaterial("Default"), matProp)));
    gameObjects[index]->addComponent(new PointLight(1.0, 0.9, 0.029));
    gameObjects[index]->getComponent<Transform>()->m_position = glm::vec3(30, 15.0f, 5.0f);
	gameObjects[index]->addComponent(new DirectionalLight(glm::normalize(glm::vec3(0.2, -1, -1)), glm::vec3(1.0f, 1.0f, 0.9f), 1.0f));
        

    gameObjects.push_back(new GameObject(new Transform()));
    index = gameObjects.size() - 1;
    gameObjects[index]->getComponent<Transform>()->m_position = glm::vec3(-30, 2.0f, -5.0f);
    gameObjects[index]->addComponent(new MeshRenderer(DATABASE::getMesh("Cube"), MaterialInstance(DATABASE::getMaterial("Default"), matProp)));
    gameObjects[index]->addComponent(new PointLight(1.0, 0.09, 0.03));

    matProp.reflectionPower = 0.4f;
    matProp.specularPower = 64.0f;
    gameObjects.push_back(new GameObject(new Transform()));
    index = gameObjects.size() - 1;
    gameObjects[index]->getComponent<Transform>()->m_position = glm::vec3(-70, 30.0f, -50.0f);
    gameObjects[index]->addComponent(new MeshRenderer(DATABASE::getMesh("Ship"), MaterialInstance(DATABASE::getMaterial("Ship"), matProp)));
    gameObjects[index]->addComponent(new PointLight(1.0, 0.09, 0.03));

    gameObjects.push_back(new GameObject(new Transform()));
    index = gameObjects.size() - 1;
    gameObjects[index]->addComponent(new SkyboxRenderer(DATABASE::getMesh("Skybox"), MaterialInstance(DATABASE::getMaterial("Skybox"), matProp)));

    
    Rife::Data::Serializer::saveScene("Main", gameObjects);

	for (size_t i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->setup();
	}

    CAMERA->updateProjection(
        60.0f,
        (float)APPLICATION->getWidth() / (float)APPLICATION->getHeight(),
        0.1f,
        10000.0f
    );

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
    delete GLOBAL_LIGHTS;
    delete SKYBOX;
}

void ScenePlayer::windowResized(const uint32_t& width, const uint32_t& height) {
    CAMERA->updateProjection(CAMERA->getFov(), (float)width / (float)height, CAMERA->getNear(), CAMERA->getFar());
}
