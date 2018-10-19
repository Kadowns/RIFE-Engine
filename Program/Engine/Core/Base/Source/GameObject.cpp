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
    updateTransform();
}

void Rife::Base::GameObject::awake(){
    for (size_t i = 0; i < m_components.size(); i++) {
        m_components[i]->awake();
    }
    updateTransform();
}

void Rife::Base::GameObject::update() {
    for (size_t i = 0; i < m_components.size(); i++) {
        m_components[i]->update();
    }
    updateTransform();
}

Rife::Base::Component* Rife::Base::GameObject::addComponent(Component* component) {
    m_components.push_back(component);
    return component;
}

void Rife::Base::GameObject::updateTransform() {
    m_transform.matrix = glm::mat4(1.0f);
    m_transform.matrix = glm::translate(m_transform.matrix, m_transform.position);
    m_transform.matrix = glm::rotate(m_transform.matrix, m_transform.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    m_transform.matrix = glm::rotate(m_transform.matrix, m_transform.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    m_transform.matrix = glm::rotate(m_transform.matrix, m_transform.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    m_transform.matrix = glm::scale(m_transform.matrix, m_transform.scale);
}
