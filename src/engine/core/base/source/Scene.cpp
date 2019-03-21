#include <Scene.h>

namespace Rife::Base {

    Scene* Scene::s_instance = nullptr;


    Scene* Scene::currentScene() {
        return s_instance;
    }

    GameObject* Scene::addGameObject() {
        if (s_instance == nullptr)
            return nullptr;

        auto go = new GameObject();
        s_instance->m_createdGameObjects.emplace(go);
        return go;
    }

    void Scene::loadScene() {
        s_instance = this;
        onLoadScene();
    }
}