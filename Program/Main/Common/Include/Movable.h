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

            camera->yaw += x;
            camera->pitch += y;

            if (camera->pitch > 89.0f) {
                camera->pitch = 89.0f;
            }
            else if (camera->pitch < -89.0f) {
                camera->pitch = -89.0f;
            }

            glm::vec3 front;
            front.x = cos(glm::radians(camera->pitch)) * cos(glm::radians(camera->yaw));
            front.y = sin(glm::radians(camera->pitch));
            front.z = cos(glm::radians(camera->pitch)) * sin(glm::radians(camera->yaw));
            camera->cameraFront = glm::normalize(front);
            camera->p_gameObject->getTransform()->m_front = camera->cameraFront;
        }

		void awake() {
			transform = p_gameObject->getTransform();
			input = KEYBOARD;
            MOUSE->setUpdatePositionCallback(this, mouseMoveCallback);
		}

		void update() {
			float dt = TIME->getLastFrameTime();

			if (input->isDown(GLFW_KEY_A)) {
				transform->m_position -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed * dt;
			}
			else if (input->isDown(GLFW_KEY_D)){
				transform->m_position += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed * dt;
			}
			else if (input->isDown(GLFW_KEY_W)) {
				transform->m_position += cameraFront * speed * dt;
			}
			else if (input->isDown(GLFW_KEY_S)){
				transform->m_position -= cameraFront * speed * dt;
			}
		}
	};
}