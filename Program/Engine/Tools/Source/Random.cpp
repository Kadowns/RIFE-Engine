#include <Random.h>

#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>
#include <cstdlib>

namespace Rife::Tools {


    float Random::range(float min, float max) {
        return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX / max)) + min;
    }

    int Random::range(int min, int max) {
        return (rand() / (RAND_MAX / max)) + min;
    }

    int Random::number(int value) {
        return rand() % value;
    }

}