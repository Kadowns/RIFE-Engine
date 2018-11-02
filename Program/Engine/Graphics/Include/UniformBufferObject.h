#pragma once

#include <ShaderItem.h>
#include <VulkanData.h>

#include <RifeMath.h>

namespace Rife::Graphics {
    namespace Ubo {
        struct uMaterialProperties final {

            glm::vec4 color;
            float specularPower;
            float tiling;
        };
    }
}
