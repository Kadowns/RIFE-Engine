#pragma once

#include <GraphicsDependencys.h>
#include <array>

namespace gph {

	struct Vertex {
		Eigen::Vector3f position;
		Eigen::Vector4f color;

		static VkVertexInputBindingDescription getBindingDescription();
		static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
	};
}