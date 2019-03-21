#pragma once

#include <PerlinNoise.h>
#include <Mesh.h>

namespace Scripts {

    struct TerrainCreateInfo {

        struct TextureWeights {
            float_t min, max;
        };

        TextureWeights weights[4];

        struct {
            uint8_t x, z;
        } extent;

        uint32_t seed;
        float_t scale, persistence, lacunarity;
        uint8_t octaves;
    };


    class TerrainGenerator {
    public:
        static Rife::Graphics::Mesh* generateTerrain(const TerrainCreateInfo& terrainCreateInfo);

    private:

        static float_t** generateNoiseMap(const TerrainCreateInfo& terrainCreateInfo, Rife::Math::PerlinNoise perlin);


    };
}