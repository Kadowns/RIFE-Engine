#pragma once

#include <RifeTools.h>
using namespace Rife::Base;

namespace Script {

	class Movable : public Component {

	private:

		float moveSpeed = 10.0f;
        float rotateSpeed = 50.0f;

        glm::vec3 front, up;

		Transform* transform;
		Rife::Tools::Keyboard* input;

	public:

        static void mouseMoveCallback(void* caller, double x, double y) {
            auto camera = reinterpret_cast<Movable*>(caller);
			camera->updateCameraDirection(x, y);         
        }

		void updateCameraDirection(double x, double y) {
            transform->m_rotation *= glm::quat(glm::radians(glm::vec3(y, -x, 0)));
            glm::normalize(transform->m_rotation);
		}

		void awake() {
			transform = p_gameObject->getComponent<Transform>();
			input = KEYBOARD;
            MOUSE->setUpdatePositionCallback(this, mouseMoveCallback);
		}

		void update() {

			if (input->isDown(GLFW_KEY_LEFT_SHIFT)) {
				moveSpeed = 30.0f;
			}
			else {
				moveSpeed = 10.0f;
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