#pragma once
#include <RifeMath.h>


namespace Rife::Base {

	struct Transform {

		glm::vec3 m_position;
        glm::vec3 m_eulerRotation;
        glm::quat m_rotation;
        glm::vec3 m_scale;

        Transform() : m_position(glm::vec3(0.0f)), m_eulerRotation(glm::vec3(0.0f)), m_rotation(glm::vec3(0.0f)), m_scale(glm::vec3(1.0f)) {}

        glm::mat4 getModelMatrix() {
            glm::mat4 translate = glm::translate(glm::mat4(1.0f), m_position);
            glm::mat4 rotate = glm::mat4_cast(m_rotation);
            glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_scale);
            return translate * rotate * scale;
        }
	};
}