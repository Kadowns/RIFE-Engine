#pragma once

#include <Component.h>

#include <map>

namespace Rife::Base {

    class GameObject : public Object {

    public:
        
        GameObject();
        GameObject(const char* name);
        ~GameObject();
        
        Component* addComponent(Component*);
       
        template<typename T>
        T* getComponent();

        void serialize(std::ofstream& file);

        void setActive(bool value);

        bool isActive() { return m_active; }

        void onInit();
        void onAwake();
        void onUpdate();
        void onLateUpdate();        

    private:
        
        bool m_active = true;

        std::map<std::type_index, Component*> m_components;
    };
    
    template<typename T>
    inline T* GameObject::getComponent() {

        std::type_index index(typeid(T));

        if (m_components.count(std::type_index(typeid(T))) != 0) {
            return static_cast<T*>(m_components[index]);
        }
        return nullptr;
    }

}