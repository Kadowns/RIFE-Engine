#pragma once
#include <MathIncludes.h>
#include <PerlinNoise.h>

namespace Rife::Math {

    template<class T>
    const T& clamp(const T& x, const T& upper, const T& lower) {
        return std::min(upper, std::max(x, lower));
    }

};

namespace std {

    string to_string(glm::vec2 vec);
    string to_string(glm::vec3 vec);
    string to_string(glm::vec4 vec);
    string to_string(glm::quat);

}
