#pragma once

#include <RifeMath.h>
#include <GraphicsLibs.h>
#include <array>

namespace Rife::Graphics {

	struct Vertex {
        glm::vec3 position;
		glm::vec4 color;
		glm::vec3 normal;

		static VkVertexInputBindingDescription getBindingDescription();
		static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
	};
}