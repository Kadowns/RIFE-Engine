#pragma once
#include <Rm.h>


namespace Rife::Base {

	struct Transform {
        glm::mat4 matrix;
		glm::vec3 position;
		glm::vec3 rotation;
	};
}