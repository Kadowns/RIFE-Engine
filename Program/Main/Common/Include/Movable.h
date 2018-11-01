#pragma once

#include <RifeTools.h>
using namespace Rife::Base;

namespace Script {

	class Movable : public Component {

	private:

		float speed = 10.0f;

        float yaw = 0, pitch = 0;

		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		Transform* transform;
		Rife::Tools::Keyboard* input;

	public:

        static void mouseMoveCallback(void* caller, double x, double y) {
            auto camera = reinterpret_cast<Movable*>(caller);
			camera->updateCameraDirection(x, y);
           
        }

		void updateCameraDirection(double x, double y) {
			yaw += x;
			pitch += y;

			if (pitch > 89.0f) {
				pitch = 89.0f;
			}
			else if (pitch < -89.0f) {
				pitch = -89.0f;
			}

			glm::vec3 front;
			front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
			front.y = sin(glm::radians(pitch));
			front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
			cameraFront = glm::normalize(front);
			p_gameObject->getTransform()->m_front = cameraFront;
		}

		void awake() {
			transform = p_gameObject->getTransform();
			input = KEYBOARD;
            MOUSE->setUpdatePositionCallback(this, mouseMoveCallback);
		}

		void update() {

			if (input->isDown(GLFW_KEY_LEFT_SHIFT)) {
				speed = 30.0f;
			}
			else {
				speed = 10.0f;
			}

			glm::vec3 moveDirection(0.0f);
			if (input->isDown(GLFW_KEY_A)) {
				moveDirection -= glm::normalize(glm::cross(cameraFront, cameraUp));
			}
			else if (input->isDown(GLFW_KEY_D)){
				moveDirection += glm::normalize(glm::cross(cameraFront, cameraUp));
			}
			if (input->isDown(GLFW_KEY_W)) {
				moveDirection += cameraFront;
			}
			else if (input->isDown(GLFW_KEY_S)){
				moveDirection -= cameraFront;
            }
            if (input->isDown(GLFW_KEY_E)) {
				moveDirection += cameraUp;
            }
            else if (input->isDown(GLFW_KEY_Q)) {
				moveDirection -= cameraUp;
            }
			if (moveDirection.x != 0 || moveDirection.y != 0 || moveDirection.z != 0) {
				move(glm::normalize(moveDirection));
			}		
		}


		void move(glm::vec3 direction) {
			transform->m_position += direction * speed * (float)TIME->getLastFrameTime();
		}
	};
}