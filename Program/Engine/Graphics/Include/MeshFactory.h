#pragma once

#include <Mesh.h>

namespace Rife::Graphics {

    class MeshFactory {
    public:
        static Mesh* createCube();
        
        static Mesh* createPlane(uint8_t width, uint8_t depth);

		static Mesh* createPolarSphere(uint8_t resolution);

		static Mesh* createNormalizedSphere(uint8_t resolution);

        static Mesh* loadMesh(const char* path);

    private:

        static std::vector<glm::vec3> calculateNormals(std::vector<glm::vec3>& positions, std::vector<uint32_t>& indices);

    };
}