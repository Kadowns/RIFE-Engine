#pragma once
#include <cstdint>
#include <vector>
#include <queue>
#include <Event.h>
#include <GameObject.h>


namespace Rife::Base {

    class Scene {
    private:
        static Scene* s_instance;

    protected:

        std::vector<GameObject*> m_gameObjects;
        std::queue<GameObject*> m_createdGameObjects, m_destroyedGameObjects;
        virtual void onLoadScene() = 0;

    public:

        static Scene* currentScene();

        static GameObject* addGameObject();

        void loadScene();
        
        virtual void onUpdate() = 0;
        virtual void onLateUpdate() = 0;
        virtual void onRender() = 0;
        virtual void onTerminate() = 0;
        Event<const uint32_t&, const uint32_t&> onWindowResized;
    };
}

//-----------------------------------------------------------such a shame
/*#define RIFE_SUBSCRIBE_EVENT_FUNCTION(name)                                             \
private:                                                                                \
    Rife::Base::Event<>::EventListener rife_function_##name##_listener;                 \
public:                                                                                 \
    void rife_subscribe_##name##() {                                                    \
        rife_function_##name##_listener = [this]() { this->##name##(); };               \
        Rife::Base::Scene::currentScene()->name += &rife_function_##name##_listener;    \
    }                                                                                   \
    void rife_unsubscribe_##name##(){                                                   \
        Rife::Base::Scene::currentScene()->name -= &rife_function_##name##_listener;    \
    }  void name()                              */                                        
                                                                                 
//-----------------------------------------------------------           