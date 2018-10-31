#pragma once

#include <Mesh.h>

namespace Rife::Graphics {

    class MeshFactory {
    public:
        static Mesh* createCube();
        
        static Mesh* createPlane(uint8_t width, uint8_t depth);

    };
}