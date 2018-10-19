#include <Component.h>

void Rife::Base::Component::update() {
    if (!m_awake) {
        if (!m_setup) {
            setup();
        }
        awake();
    }
}

void Rife::Base::Component::setParentObject(Object* parent) {
    p_parentObject = parent;
}
