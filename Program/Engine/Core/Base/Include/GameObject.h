#pragma once
#ifndef INCLUDE_GAMEOBJECT
#define INCLUDE_GAMEOBJECT

#include <Component.h>
#include <Transform.h>
#include <vector>

namespace Rife::Base {

    class GameObject : Object {

    public :
        
        GameObject();
        ~GameObject();
        
        void setup();
        void awake();
        void update();
        

       Component* addComponent(Component*);

       Transform* getTransform() { return &m_transform; }

    protected:

        Transform m_transform;

    private:

        std::vector<Component*> m_components;

    };
}
#endif // !INCLUDE_GAMEOBJECT
