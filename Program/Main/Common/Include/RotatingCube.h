#pragma once

#include <RifeCore.h>

using namespace Rife::Base;

namespace Script {

    class RotatingCube : public Component {
    private:
        Transform* transform;
        glm::vec3 rotation;
    public:

        float getRand(int maxValue) {
            return (rand() % maxValue * 2) - maxValue;
        }

        void awake() {

            transform = p_gameObject->getTransform();
            rotation = glm::vec3(getRand(10), getRand(90), getRand(10));
            printf("\nRotation: x:%f, y:%f, z:%f", rotation.x, rotation.y, rotation.z);
        }

        void update() {

            transform->m_eulerRotation = rotation * (float)glm::radians(TIME->time());
            transform->m_rotation = glm::quat(transform->m_eulerRotation);
        }
    };
}