#include <GameObject.h>

Rife::Base::GameObject::GameObject() {}

Rife::Base::GameObject::~GameObject() {
    for (int i = 0; i < m_components.size(); i++) {
        delete m_components[i];
    }
}

void Rife::Base::GameObject::setup() {
    for (size_t i = 0; i < m_components.size(); i++) {
        m_components[i]->setup();
    }
}

void Rife::Base::GameObject::awake(){
    for (size_t i = 0; i < m_components.size(); i++) {
        m_components[i]->awake();
    }
}

void Rife::Base::GameObject::update() {
    for (size_t i = 0; i < m_components.size(); i++) {
        m_components[i]->update();
    }
}

Rife::Base::Component* Rife::Base::GameObject::addComponent(Component* component) {
    m_components.push_back(component);
    return component;
}
