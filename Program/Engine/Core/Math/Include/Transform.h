#pragma once
#include <Rm.h>


namespace Rife::Base {

	struct Transform {
        glm::mat4 matrix = glm::mat4(1.0f);
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 rotation = glm::vec3(0.0f);
        glm::vec3 scale = glm::vec3(1.0f);
	};
}