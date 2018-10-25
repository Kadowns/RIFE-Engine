#pragma once
#include <RifeMath.h>

namespace Rife::Graphics {

	struct MaterialProperties {
		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specular;
	};
}