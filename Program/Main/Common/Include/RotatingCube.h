#pragma once

#include <RifeCore.h>

using namespace Rife::Base;

namespace Script {

    class RotatingCube : public Component {
    private:
        Transform* transform;

    public:

        void awake() {
            transform = p_gameObject->getTransform();
        }

        void update() {

            auto time = TIME->time();
            transform->m_position += glm::vec3(sin(glm::radians(TIME->time())) * 0.1f) * 0.01f;
            transform->m_eulerRotation = glm::radians(glm::vec3(time * 95.0f,time * 5.0f, time * 0.0f));
            transform->m_rotation = glm::quat(transform->m_eulerRotation);
        }
    };
}