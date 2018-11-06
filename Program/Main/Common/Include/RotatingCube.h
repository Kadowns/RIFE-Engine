#pragma once

#include <RifeCore.h>
#include <RifeGraphics.h>
#include <Random.h>

using namespace Rife::Base;
using namespace Rife::Graphics;

namespace Script {

    class RotatingCube : public Component {
    private:
       
        Transform* transform;
        glm::vec3 rotation;
    public:

        RotatingCube() {
            m_name = "RotatingCube";
        }

        void serialize(std::ofstream& file) {
            
        }

        void awake() {

            transform = getComponent<Transform>();
            rotation = glm::vec3(Random::range(-50, 50), Random::range(-20, 20), Random::range(-20, 20));
        }

        void update() {
            transform->m_rotation *= glm::quat(glm::radians(rotation * (float)TIME->getLastFrameTime()));
            glm::normalize(transform->m_rotation);
        }
    };
}