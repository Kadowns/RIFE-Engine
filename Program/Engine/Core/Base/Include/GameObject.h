#pragma once

#include <Component.h>
#include <Transform.h>
#include <vector>

namespace Rife::Base {

    class GameObject : Object {

    public:
        
        GameObject();
        ~GameObject();
        
        void setup();
        void awake();
        void update();
        
       Component* addComponent(Component*);

       Transform* getTransform() { return &m_transform; }

    private:

        Transform m_transform;
        std::vector<Component*> m_components;
    };
}
