#pragma once

#include <GraphicsDependencys.h>

namespace Graphics {
	struct UniformBufferObject {
		glm::mat4 projection;
        glm::mat4 view;
        glm::mat4 model;
	};
}