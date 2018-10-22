#pragma once

#include <RifeMath.h>
#include <GraphicsLibs.h>
#include <array>

namespace Rife::Graphics {

	struct Vertex {
        glm::vec3 position;
		glm::vec4 color;

		static VkVertexInputBindingDescription getBindingDescription();
		static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
	};
}