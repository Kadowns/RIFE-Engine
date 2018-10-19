#pragma once
#ifndef INCLUDE_UBO
#define INCLUDE_UBO
#include <GraphicsDependencys.h>

namespace Rife::Graphics {
    namespace Ubo {
        struct Mvp {
            glm::mat4 mvp;
        };
    }
}

#endif // !INCLUDE_UBO

