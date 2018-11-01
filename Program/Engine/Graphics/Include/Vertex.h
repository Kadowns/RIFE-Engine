#pragma once

#include <RifeMath.h>
#include <VulkanInclude.h>
#include <array>

namespace Rife::Graphics {

	struct Vertex {
        glm::vec3 position;
		glm::vec3 normal;
        glm::vec2 texCoord;

		static VkVertexInputBindingDescription getBindingDescription();
		static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
	};
}