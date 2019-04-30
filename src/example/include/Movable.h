#pragma once

#include <RifeCore.h>
#include <RifeTools.h>
#include <RifeGraphics.h>
#include <RotatingCube.h>

using namespace Rife::Base;
using namespace Rife::Graphics;
using namespace Rife::Tools;

namespace Scripts {

	class Movable : public Component {

	private:

        const float baseSpeed = 50.0f;
		float moveSpeed;
        float rotateSpeed;

        bool mouseDrag = false;

        glm::vec3 front, up, right;

		Transform* transform;
	    Keyboard* input;
        Mouse* mouse;        

        OnMouseMove::EventListener callback;

	public:

        Movable() {
            m_name = "Movable";
        }

        ~Movable() {
        }

        void serialize(std::ofstream& file) {
            
        }

        void onMouseMove(double x, double y) {

            if (mouseDrag) {
                transform->m_rotation *= glm::quat(glm::radians(glm::vec3(y, -x, 0)));
                glm::normalize(transform->m_rotation);
            }          
        }

        virtual void onAwake() override {
			transform = getComponent<Transform>();
			input = KEYBOARD;
            mouse = MOUSE;
            callback = [this](double x, double y) {
                this->onMouseMove(x, y);
            };
            mouse->onMouseMove() += &callback;
		}

		virtual void onUpdate() override {

            if (mouse->isPressed(GLFW_MOUSE_BUTTON_LEFT)) {
                mouse->setCursor(false);            
                mouseDrag = true;
            }
            else if (mouse->isReleased(GLFW_MOUSE_BUTTON_LEFT)) {
                mouse->setCursor(true);
                mouseDrag = false;
            }

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

            if (input->isDown(GLFW_KEY_G)) {
                auto go = Scene::addGameObject();
                go->addComponent(new Transform());
                Ubo::MaterialProperties matProp = {};
                matProp.color = glm::vec4(Random::range(0.5f, 1.0f), Random::range(0.5f, 1.0f), Random::range(0.5f, 1.0f), 1.0f);
                matProp.specularPower = Random::range(1.0f, 256.0f);
                matProp.reflectionPower = Random::range(0.5f, 1.0f);

                go->addComponent(new MeshRenderer(DATABASE::getMesh("Cube"), MaterialInstance(DATABASE::getMaterial("Metal"), matProp)));
                go->addComponent(new RotatingCube());
                go->getComponent<Transform>()->position = transform->position + (transform->getFront() * 10.0f);
            }

            float rotateAmount = 0.0f;
            front = transform->getFront();
            up = transform->getUp();
            right = transform->getRight();
			glm::vec3 moveDirection(0.0f);

			if (input->isDown(GLFW_KEY_A)) {
                moveDirection -= right;
			}
			else if (input->isDown(GLFW_KEY_D)){
                moveDirection += right;
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
			transform->position += direction * moveSpeed * (float)TIME->getLastFrameTime();
		}

        void rotateZ(double amount) {
            transform->m_rotation *= glm::quat(glm::radians(glm::vec3(0.0f, 0.0f, amount * TIME->getLastFrameTime())));
            glm::normalize(transform->position);
        }
	};
}