#pragma once
#ifndef INCLUDE_UBO
#define INCLUDE_UBO
#include <GraphicsLibs.h>

namespace Rife::Graphics {
    namespace Ubo {
        struct Transform {
            glm::mat4 vp;
			glm::mat4 m;
			glm::vec4 cameraPos;
        };
    }
}

#endif // !INCLUDE_UBO

