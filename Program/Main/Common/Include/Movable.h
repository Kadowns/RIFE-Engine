#pragma once

#include <RifeTools.h>
using namespace Rife::Base;

namespace Script {

	class Movable : public Component {

	private:

		float speed = 10.0f;

		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		Transform* transform;
		Rife::Tools::Keyboard* input;

	public:

		void awake() {
			transform = p_gameObject->getTransform();
			input = KEYBOARD;
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