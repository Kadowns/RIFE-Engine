#pragma once

#include <GraphicsDependencys.h>
#include <array>

namespace gph {

	struct Vertex {
        glm::vec3 position;
		glm::vec4 color;

		static VkVertexInputBindingDescription getBindingDescription();
		static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
	};
}