#pragma once

#include <Component.h>
#include <typeindex>
#include <map>

namespace Rife::Base {

    class GameObject : Object {

    public:
        
        GameObject();
        GameObject(Component* transform);
        ~GameObject();
        
        void setup();
        void awake();
        void update();
        
        Component* addComponent(Component*);
       
        template<typename T>
        T* getComponent();


    private:
        
        std::map<std::type_index, Component*> m_components;
    };
    
    template<typename T>
    inline T* GameObject::getComponent() {

        std::type_index index(typeid(T));

        if (m_components.count(std::type_index(typeid(T))) != 0) {
            return reinterpret_cast<T*>(m_components[index]);
        }
        return nullptr;
    }

}