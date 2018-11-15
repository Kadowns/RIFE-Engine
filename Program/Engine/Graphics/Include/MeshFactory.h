#pragma once

#include <MeshBuilder.h>

namespace Rife::Graphics {

    class MeshFactory {
    public:
        static Mesh* createCube();

		

		static Mesh* createSkybox();
        
        static Mesh* createPlane(uint8_t width, uint8_t depth);

        static Mesh* createTerrain(uint8_t width, uint8_t depth, size_t seed);

		static Mesh* createPolarSphere(uint8_t resolution);

		static Mesh* createNormalizedSphere(uint8_t resolution);

        static Mesh* loadMesh(const char* path);

        static std::vector<glm::vec3> calculateNormals(std::vector<glm::vec3>& positions, std::vector<uint32_t>& indices);

        static void generateCubeInfo(uint8_t resolution, std::vector<glm::vec3>& positions, std::vector<glm::vec2>& uvs, std::vector<uint32_t>& indices);
        
    };
}