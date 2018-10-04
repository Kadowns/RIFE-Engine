#pragma once

#include <GraphicsDependencys.h>

namespace gph {
	struct UniformBufferObject {
		Eigen::Matrix4f projection;
		Eigen::Matrix4f view;
		Eigen::Matrix4f model;
	};
}