#pragma once

#include <ShaderItem.h>
#include <VulkanData.h>

#include <RifeMath.h>

namespace Rife::Graphics {
    namespace Ubo {
        struct MaterialProperties final {

            glm::vec4 color;
            float specularPower;
            float tiling;
			float reflectionPower;
        };
    }
}
