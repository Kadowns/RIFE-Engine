#pragma once
#include <RifeMath.h>

namespace Rife::Graphics {

	struct Light {
		glm::vec3 direction;
		glm::vec3 position;
		glm::vec3 ambient;
		glm::vec3 diffuse;
	};

}