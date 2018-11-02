#include <GameObject.h>

Rife::Base::GameObject::GameObject() {}

Rife::Base::GameObject::GameObject(Component* transform) {
    addComponent(transform);
}

Rife::Base::GameObject::~GameObject() {
    for (auto it : m_components) {
        delete it.second;
    }
}

void Rife::Base::GameObject::setup() {
    for (auto it : m_components) {
        it.second->setup();
    }
}

void Rife::Base::GameObject::awake(){
    for (auto it : m_components) {
        it.second->awake();
    }
}

void Rife::Base::GameObject::update() {
    for (auto it : m_components) {
        it.second->update();
    }
}

Rife::Base::Component* Rife::Base::GameObject::addComponent(Component* component) {
    m_components[std::type_index(typeid(*component))] = component;
    component->setParentObject(this);
    return component;
}
