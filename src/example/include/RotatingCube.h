#pragma once

#include <RifeCore.h>
#include <RifeGraphics.h>
#include <Random.h>

using namespace Rife::Base;
using namespace Rife::Graphics;

namespace Scripts {

    class RotatingCube : public Component {
    private:
       
        Transform* transform;
        glm::vec3 rotation;
    public:

        RotatingCube() {
            m_name = "RotatingCube";
        }

        ~RotatingCube() {         
        }

        void serialize(std::ofstream& file) {
            
        }

        virtual void onAwake() override {
            transform = getComponent<Transform>();
            rotation = glm::vec3(Rife::Tools::Random::range(-50, 50), Rife::Tools::Random::range(-20, 20), Rife::Tools::Random::range(-20, 20));
        }

        virtual void onUpdate() override {
            transform->m_rotation *= glm::quat(glm::radians(rotation * (float)TIME->getLastFrameTime()));
            glm::normalize(transform->m_rotation);
        }
    };
}