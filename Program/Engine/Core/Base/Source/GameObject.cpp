#include <GameObject.h>

namespace Rife::Base {
    GameObject::GameObject() {}

    GameObject::GameObject(Component* transform) {
        m_name = "GameObject";
        addComponent(transform);
    }

    GameObject::~GameObject() {
        for (auto it : m_components) {
            delete it.second;
        }
    }

    void GameObject::setup() {
        for (auto it : m_components) {
            it.second->setup();
        }
    }

    void GameObject::awake() {
        for (auto it : m_components) {
            it.second->awake();
        }
    }

    void GameObject::update() {
        for (auto it : m_components) {
            it.second->update();
        }
    }

    Component* GameObject::addComponent(Component* component) {
        m_components[std::type_index(typeid(*component))] = component;
        component->setParentObject(this);
        return component;
    }

    void GameObject::serialize(std::ofstream & file) {
        file << (m_name + ":\n").c_str();
        for (auto it : m_components) {
            file << ("  c:" + it.second->getName() + ":\n").c_str();
            it.second->serialize(file);
        }
        file << "-\n";
    }
}

