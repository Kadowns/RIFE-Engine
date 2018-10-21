#include <GameObject.h>

Rife::Base::GameObject::GameObject() {}

Rife::Base::GameObject::~GameObject() {
    for (int i = 0; i < m_components.size(); i++) {
        delete m_components[i];
    }
}

void Rife::Base::GameObject::setup() {
    
    updateTransform();
}

void Rife::Base::GameObject::awake(){
    
    updateTransform();
}

void Rife::Base::GameObject::update() {
    
    updateTransform();
}

Rife::Base::Component* Rife::Base::GameObject::addComponent(Component* component) {
    m_components.push_back(component);
    component->setParentObject(this);
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
