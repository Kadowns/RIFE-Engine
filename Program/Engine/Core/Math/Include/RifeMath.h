#pragma once
#include <MathIncludes.h>
#include <PerlinNoise.h>

namespace Rife::Math {

    template<typename T>
    const T clamp(const T& x, const T& upper, const T& lower) {
        return std::min(upper, std::max(x, lower));
    }

    template<typename T>
    const T inverseLerp(const T& first, const T& second, const T& value) {
        return (value - first) / (second - first);
    }

    template<typename T>
    const T lerp(const T& first, const T& second, const float& percent) {
        return first + (second - first) * percent;
    }

    template<typename T>
    const T linear(const T& min, const T& max, const T& value, const bool& inverse) {
        T range = max - min;
        T result = (value - min) / range;
        result = clamp(result, T(1), T(0));
        return inverse ? T(1) - result : result;
    }

    template<typename T>
    const T piramid(const T& min, const T& max, const T& value) {     
        T mid = (min + max) / T(2);
        return value <= mid ?
            linear(min, mid, value, false) :
            linear(mid, max, value, true);
    }
};

namespace std {

    string to_string(glm::vec2 vec);
    string to_string(glm::vec3 vec);
    string to_string(glm::vec4 vec);
    string to_string(glm::quat);

}
