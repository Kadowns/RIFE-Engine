#pragma once
#ifndef RM
#define RM

#include <Types.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#define DEG_TO_RAD (M_PI / 180)
#define RAD_TO_DEG (180 / M_PI)


namespace rm {

    template<class T>
    const T& clamp(const T& x, const T& upper, const T& lower) {
        return std::min(upper, std::max(x, lower));
    }

};

#endif // !RM


