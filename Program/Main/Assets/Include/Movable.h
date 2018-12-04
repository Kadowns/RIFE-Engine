#pragma once

#include <RifeCore.h>
#include <RifeTools.h>
#include <RifeGraphics.h>

using namespace Rife::Base;
using namespace Rife::Graphics;
using namespace Rife::Tools;

namespace Scripts {

	class Movable : public Component {

	private:

        const float baseSpeed = 50.0f;
		float moveSpeed;
        float rotateSpeed;

        glm::vec3 front, up;

		Transform* transform;
	    Keyboard* input;

        MouseMoveEvent::EventListener callback;


	public:

        Movable() {
            m_name = "Movable";
        }

        void serialize(std::ofstream& file) {
            
        }

        void OnMouseMove(double x, double y) {
			updateCameraDirection(x, y);         
        }

		void updateCameraDirection(double x, double y) {
            transform->m_rotation *= glm::quat(glm::radians(glm::vec3(y, -x, 0)));
            glm::normalize(transform->m_rotation);
		}

		void awake() {
			transform = getComponent<Transform>();
			input = KEYBOARD;
            using namespace std::placeholders;
            callback = std::bind(&Movable::OnMouseMove, this, _1, _2);
            MOUSE->OnMouseMove() += &callback;
		}

		void update() {

			if (input->isDown(GLFW_KEY_LEFT_SHIFT)) {
				moveSpeed = baseSpeed * 10;
                rotateSpeed = baseSpeed * 5.0f;
			}
			else if (input->isDown(GLFW_KEY_LEFT_CONTROL)){
                moveSpeed = baseSpeed * 0.25f;
                rotateSpeed = baseSpeed * 0.25f;
			}
            else {
                moveSpeed = baseSpeed;
                rotateSpeed = baseSpeed;
            }

            float rotateAmount = 0.0f;
            front = transform->getFront();
            up = transform->getUp();
			glm::vec3 moveDirection(0.0f);

			if (input->isDown(GLFW_KEY_A)) {
				moveDirection -= glm::normalize(glm::cross(front, up));
			}
			else if (input->isDown(GLFW_KEY_D)){
				moveDirection += glm::normalize(glm::cross(front, up));
			}
			if (input->isDown(GLFW_KEY_W)) {
				moveDirection += front;
			}
			else if (input->isDown(GLFW_KEY_S)){
				moveDirection -= front;
            }
            if (input->isDown(GLFW_KEY_E)) {
                rotateAmount = -rotateSpeed;
            }
            else if (input->isDown(GLFW_KEY_Q)) {
                rotateAmount = rotateSpeed;
            }
			if (moveDirection.x != 0 || moveDirection.y != 0 || moveDirection.z != 0) {
				move(glm::normalize(moveDirection));
			}	
            if (rotateAmount != 0) {
                rotateZ(rotateAmount);
            }
		}

		void move(glm::vec3 direction) {
			transform->m_position += direction * moveSpeed * (float)TIME->getLastFrameTime();
		}

        void rotateZ(double amount) {
            transform->m_rotation *= glm::quat(glm::radians(glm::vec3(0.0f, 0.0f, amount * TIME->getLastFrameTime())));
            glm::normalize(transform->m_position);
        }
	};
}