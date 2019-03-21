#include <ScenePlayer.h>

#include <RifeAssets.h>


using namespace Scripts;

ScenePlayer::ScenePlayer(){
    
}


ScenePlayer::~ScenePlayer() {
}

void ScenePlayer::onLoadScene() {


    auto go = Scene::addGameObject();
    go->addComponent(new Transform());
    go->getComponent<Transform>()->position = glm::vec3(0.0f, 0.2f, 6.0f);
    go->addComponent(new Movable());
    go->addComponent(new Camera());
    
    
    Ubo::MaterialProperties matProp = {};
    matProp.color = glm::vec4(1.0f);
	matProp.reflectionPower = 0;
	matProp.specularPower = 0;
    matProp.tiling = 4.0f;
    
    TerrainCreateInfo terrainInfo = {};
    terrainInfo.extent.x = 64;
    terrainInfo.extent.z = 64;
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

    go = Scene::addGameObject();
    go->addComponent(new Transform());    
    go->addComponent(new MeshRenderer(
        DATABASE::getMesh("Cube"),//DATABASE::insertMesh(TerrainGenerator::generateTerrain(terrainInfo), "Terrain"),
        MaterialInstance(DATABASE::getMaterial("Terrain"), matProp)));
    auto t = go->getComponent<Transform>();
    t->position = glm::vec3(0.0f, -450.0f, 0.0f);
    //t->m_scale = glm::vec3(100.0f, 480.0f, 100.0f);

    matProp.tiling = 1.0f;
	int newSize = m_gameObjects.size() + 10;
    for (int i = m_gameObjects.size(); i < newSize; i++) {
        go = Scene::addGameObject();
        go->addComponent(new Transform());

        matProp.color = glm::vec4(Random::range(0.5f, 1.0f), Random::range(0.5f, 1.0f), Random::range(0.5f, 1.0f), 1.0f);
        matProp.specularPower = Random::range(1.0f, 256.0f);
        matProp.reflectionPower = Random::range(0.5f, 1.0f);

        go->addComponent(new MeshRenderer(DATABASE::getMesh("PolarSphere"), MaterialInstance(DATABASE::getMaterial("Metal"), matProp)));
        go->addComponent(new RotatingCube());
        t = go->getComponent<Transform>();
        t->position = glm::vec3(40.0f - 20.0f * (i % 4), 0.0f, 10.0f - 2.0f * (i + 1));
    }

    go = Scene::addGameObject();
    go->addComponent(new Transform());
    matProp.color = glm::vec4(1.0f);
    matProp.reflectionPower = 0;
    matProp.specularPower = 0;
    go->addComponent(new MeshRenderer(DATABASE::getMesh("Cube"), MaterialInstance(DATABASE::getMaterial("Default"), matProp)));
    go->addComponent(new PointLight(1.0, 0.9, 0.029));
    go->getComponent<Transform>()->position = glm::vec3(30, 15.0f, 5.0f);
    go->addComponent(new DirectionalLight(glm::normalize(glm::vec3(0.2, -1, -1)), glm::vec3(1.0f, 1.0f, 0.9f), 1.0f));
        

    go = Scene::addGameObject();
    go->addComponent(new Transform());
    go->getComponent<Transform>()->position = glm::vec3(-30, 2.0f, -5.0f);
    go->addComponent(new MeshRenderer(DATABASE::getMesh("Cube"), MaterialInstance(DATABASE::getMaterial("Default"), matProp)));
    go->addComponent(new PointLight(1.0, 0.09, 0.03));

    matProp.reflectionPower = 0.4f;
    matProp.specularPower = 64.0f;
    go = Scene::addGameObject();
    go->addComponent(new Transform());
    go->getComponent<Transform>()->position = glm::vec3(-70, 30.0f, -50.0f);
    go->addComponent(new MeshRenderer(DATABASE::getMesh("Ship"), MaterialInstance(DATABASE::getMaterial("Ship"), matProp)));
    go->addComponent(new PointLight(1.0, 0.09, 0.03));

    go = Scene::addGameObject();
    go->addComponent(new Transform());
    go->addComponent(new SkyboxRenderer(DATABASE::getMesh("Skybox"), MaterialInstance(DATABASE::getMaterial("Skybox"), matProp)));

    CAMERA->updateProjection(
        60.0f,
        (float)WINDOW->getWidth() / (float)WINDOW->getHeight(),
        0.1f,
        10000.0f
    );

}

void ScenePlayer::onUpdate() {

    while (!m_createdGameObjects.empty()) {
        auto& go = m_createdGameObjects.front();
        go->onInit();
        go->onAwake();
        m_gameObjects.push_back(go);
        m_createdGameObjects.pop();
    }


    for (auto& it : m_gameObjects) {
        it->onUpdate();
    }
}

void ScenePlayer::onLateUpdate()
{
    for (auto& it : m_gameObjects) {
        it->onLateUpdate();
    }
}

void ScenePlayer::onRender() {

	uint32_t imageIndex;
	VkResult result = VK_BASE->prepareFrame(&imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		VK_BASE->windowResized();
		return;
	}
	std::vector<VkSemaphore> waitSemaphores, signalSemaphores;
	VK_BASE->submitFrame(imageIndex, waitSemaphores, signalSemaphores);
	
	result = VK_BASE->presentFrame(imageIndex, signalSemaphores);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || *WINDOW->windowResized()) {
		*WINDOW->windowResized() = false;
		VK_BASE->windowResized();
        CAMERA->updateProjection(
            60.0f,
            (float)WINDOW->getWidth() / (float)WINDOW->getHeight(),
            0.1f,
            10000.0f
        );
    } 
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

}

void ScenePlayer::onTerminate() {
    for (int i = 0; i < m_gameObjects.size(); i++){
        delete m_gameObjects[i];
    }
    delete GLOBAL_LIGHTS;
    delete SKYBOX;
}
