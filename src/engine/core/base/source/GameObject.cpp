#include <Scene.h>


namespace Rife::Graphics {
    class Transform;
}

namespace Rife::Base {
    GameObject::GameObject() {
        m_name = "GameObject";
    }

    GameObject::GameObject(const char* name) {
        m_name = name;
    }

    GameObject::~GameObject() {
        for (auto it : m_components) {
            delete it.second;
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

    void GameObject::setActive(bool value) {
        m_active = value;
    }

    void GameObject::onInit() {
        for (auto& it : m_components) {
            it.second->onInit();
        }
    }

    void GameObject::onAwake() {
        for (auto& it : m_components) {
            it.second->onAwake();
        }
    }

    void GameObject::onUpdate() {
        for (auto& it : m_components) {
            it.second->onUpdate();
        }
    }

    void GameObject::onLateUpdate() {
        for (auto& it : m_components) {
            it.second->onLateUpdate();
        }
    }
}

