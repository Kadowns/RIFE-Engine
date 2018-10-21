#include <Component.h>


void Rife::Base::Component::setup() {
    m_setup = true;
}

void Rife::Base::Component::awake() {
    if (!m_setup) {
        setup();
    }
    m_awake = true;
}

void Rife::Base::Component::update() {
    if (!m_awake) {
        awake();
    }
}

void Rife::Base::Component::setParentObject(GameObject* parent) {
    p_gameObject = parent;
}
