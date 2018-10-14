#pragma once
#include <GraphicsDependencys.h>


namespace Entity {

	struct Transform {
		glm::vec3 position;
		glm::vec3 rotation;
	};

	struct FinalTransform {
		glm::mat4 mvp;
	};

}