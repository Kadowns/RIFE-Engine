#include <TerrainGenerator.h>

#include <MeshFactory.h>

#include <random>
#include <limits>

using namespace Rife;
using namespace Rife::Graphics;

namespace Scripts {

    Mesh* TerrainGenerator::generateTerrain(const TerrainCreateInfo& settings) {

        Math::PerlinNoise perlin(settings.seed);

        float_t** noiseMap = generateNoiseMap(settings, perlin);

        const uint8_t& width = settings.extent.x;
        const uint8_t& depth = settings.extent.z;

        float_t hw = width / 2.0f;
        float_t hd = depth / 2.0f;

        
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uvs;
        float maxH = -std::numeric_limits<float>::max();
        for (uint32_t z = 0; z < depth; z++) {
            for (uint32_t x = 0; x < width; x++) {

                float h = noiseMap[x][z];
                positions.push_back(glm::vec3(x - hw, h, z - hd));
                uvs.push_back(glm::vec2(x, z));
               
                if (h > maxH) {
                    maxH = h;
                }
            }
        }

        std::vector<glm::vec4> weights;
        for (uint32_t z = 0; z < depth; z++) {
            for (uint32_t x = 0; x < width; x++) {
                float h = noiseMap[x][z];
                h /= maxH;
                weights.push_back(glm::vec4(
                    Math::linear(settings.weights[3].min, settings.weights[3].max, h, false),
                    Math::piramid(settings.weights[2].min, settings.weights[2].max, h),
                    Math::piramid(settings.weights[1].min, settings.weights[1].max, h),
                    Math::linear(settings.weights[0].min, settings.weights[0].max, h, true)
                ));
            }
        }

        std::vector<uint32_t> indices;
        for (uint8_t z = 0; z < depth - 1; z++) {
            for (uint8_t x = 0; x < width - 1; x++) {
                uint32_t zero = x + z * width;
                uint32_t one = (x + 1) + z * width;
                uint32_t two = x + (z + 1) * width;
                uint32_t three = (x + 1) + (z + 1) * width;

                indices.push_back(zero);
                indices.push_back(three);
                indices.push_back(one);

                indices.push_back(zero);
                indices.push_back(two);
                indices.push_back(three);
            }
        }

        std::vector<glm::vec3> normals = MeshFactory::calculateNormals(positions, indices);

        return MeshBuilder()
            .addVecAttribute(positions)
            .addVecAttribute(normals)
            .addVecAttribute(uvs)
            .addVecAttribute(weights)
            .setIndices(indices)
            .create(positions.size());
    }


    float_t** TerrainGenerator::generateNoiseMap(const TerrainCreateInfo& settings, Math::PerlinNoise perlin) {

        float** noiseMap;
        noiseMap = new float*[settings.extent.x];
        for (uint8_t i = 0; i < settings.extent.x; i++) {
            noiseMap[i] = new float[settings.extent.z];
        }


        std::default_random_engine generator(settings.seed);
        std::uniform_int_distribution<int> distributor(-1000000, 1000000);

        std::vector<glm::vec2> octaveOffset(settings.octaves);
        for (auto& it : octaveOffset) {
            int x = distributor(generator);
            int z = distributor(generator);
            it = std::move(glm::vec2(x, z));
        }

        float_t maxNoiseHeight = -std::numeric_limits<float>::max();
        float_t minNoiseHeight = std::numeric_limits<float>::max();

        for (uint8_t z = 0; z < settings.extent.z; z++) {
            for (uint8_t x = 0; x < settings.extent.x; x++) {

                float_t amplitude = 1;
                float_t frequency = 1;
                float_t noiseHeight = 0;

                for (uint8_t octave = 0; octave < settings.octaves; octave++) {

                    float sampleX = x / settings.scale * frequency + octaveOffset[octave].x;
                    float sampleZ = z / settings.scale * frequency + octaveOffset[octave].y;

                    float perlinValue = perlin.noise(sampleX, sampleZ, 0.5) * 2 - 1;
                    noiseHeight += perlinValue * amplitude;

                    amplitude *= settings.persistence;
                    frequency *= settings.lacunarity;
                }

                if (noiseHeight > maxNoiseHeight) {
                    maxNoiseHeight = noiseHeight;
                }
                else if (noiseHeight < minNoiseHeight) {
                    minNoiseHeight = noiseHeight;
                }

                noiseMap[x][z] = noiseHeight;
            }
        }

        for (uint8_t z = 0; z < settings.extent.z; z++) {
            for (uint8_t x = 0; x < settings.extent.x; x++) {
                noiseMap[x][z] = Math::inverseLerp(minNoiseHeight, maxNoiseHeight, noiseMap[x][z]);
            }
        }

        return noiseMap;
    }
}


