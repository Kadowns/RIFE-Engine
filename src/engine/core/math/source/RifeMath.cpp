#include <RifeMath.h>
#include <string>

namespace std {

    string to_string(glm::vec2 vec) {
        return to_string(vec.x) + " " + to_string(vec.y);
    }

    string to_string(glm::vec3 vec) {
        return to_string(glm::vec2(vec)) + " " + to_string(vec.z);
    }

    string to_string(glm::vec4 vec) {
        return to_string(glm::vec3(vec)) + " " + to_string(vec.w);
    }

    string to_string(glm::quat quat) {
        return to_string(glm::vec4(quat.x, quat.y, quat.z, quat.w));
    }
}