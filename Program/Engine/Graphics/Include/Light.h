#pragma once
#include <RifeMath.h>

namespace Rife::Graphics {

	struct Light {
		glm::vec4 direction;
		glm::vec4 ambient;
		glm::vec4 diffuse;

		//xyz = color, w = power
		glm::vec4 specular;
	};

}