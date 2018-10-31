#include <MeshFactory.h>

namespace Rife::Graphics {

    Mesh* MeshFactory::createCube() {

        std::vector<Vertex> vertices = {

            {{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
            {{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
            {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},

            {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
            {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},

            {{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
            {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},

            {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
            {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
            {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},

            {{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
            {{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},

            {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
            {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}}
        };

        std::vector<uint32_t> indices = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            8, 9, 10, 10, 11, 8,
            12, 13, 14, 14, 15, 12,
            16, 17, 18, 18, 19, 16,
            20, 21, 22, 22, 23, 20
        };
        return new Mesh(vertices, indices);
    }

    Mesh* MeshFactory::createPlane(uint8_t width, uint8_t depth) {

        float_t hw = width / 2.0f;
        float_t hd = depth / 2.0f;

        std::vector<glm::vec3> positions;
        for (uint32_t z = 0; z < depth; z++) {
            for (uint32_t x = 0; x < width; x++) {
                positions.push_back(glm::vec3(x - hw, 0.0f, z - hd));
            }
        }

        std::vector<uint32_t> indices;
        for (uint32_t z = 0; z < depth - 1; z++) {
            for (uint32_t x = 0; x < width - 1; x++) {
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

        std::vector<glm::vec3> normals(positions.size(), glm::vec3(0));
        for (uint32_t i = 0; i < indices.size(); i += 3) {
            uint32_t i1 = indices[i];
            uint32_t i2 = indices[i + 1];
            uint32_t i3 = indices[i + 2];

            glm::vec3 v1 = positions[i1];
            glm::vec3 v2 = positions[i2];
            glm::vec3 v3 = positions[i3];

            glm::vec3 side1 = v2 - v1;
            glm::vec3 side2 = v3 - v1;

            glm::vec3 normal = glm::cross(side1, side2);

            normals[i1] += normal;
            normals[i2] += normal;
            normals[i3] += normal;
        }

        for (uint32_t i = 0; i < normals.size(); i++) {
            glm::normalize(normals[i]);
        }

        float tx = 1.0f / width * 2;
        float ty = 1.0f / depth * 2;
        std::vector<glm::vec2> texCoords;
        for (int z = 0; z < depth; z++) {
            for (int x = 0; x < width; x++) {
                texCoords.push_back(glm::vec2(x * tx, z * ty));
            }
        }

        std::vector<Vertex> vertices;
        vertices.resize(positions.size());
        for (uint32_t i = 0; i < vertices.size(); i++) {
            vertices[i].position = positions[i];
            vertices[i].texCoord = texCoords[i];
            vertices[i].normal = normals[i];
            vertices[i].color = glm::vec4(1.0f);
        }

        return new Mesh(vertices, indices);
    }

	Mesh* MeshFactory::createSphere(uint8_t slices, uint8_t stacks) {

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> texCoords;

		auto pi = 3.141592;

		for (uint8_t slice = 0; slice <= slices; slice++) {
			double theta = slice *  pi / slices;
			double sinTheta = sin(theta);
			double cosTheta = cos(theta);

			for (uint8_t stack = 0; stack <= stacks; stack++) {
				double phi = stack * 2 * pi / stacks;
				double sinPhi = sin(phi);
				double cosPhi = cos(phi);
				float x = (float)(cosPhi * sinTheta);
				float y = (float)(cosTheta);
				float z = (float)(sinPhi * sinTheta);
				float s = 1.0f - (stack / (float)stacks);
				float t = 1.0f - (slice / (float)slices);
				texCoords.push_back(glm::vec2(s, t));
				positions.push_back(glm::vec3(x, y, z));
			}
		}

		std::vector<uint32_t> indices;
		for (int z = 0; z < slices; z++) {
			for (int x = 0; x < stacks; x++) {
				uint32_t zero = x + z * stacks;
				uint32_t one = (x + 1) + z * stacks;
				uint32_t two = x + (z + 1) * stacks;
				uint32_t three = (x + 1) + (z + 1) * stacks;

				indices.push_back(zero);
				indices.push_back(one);
				indices.push_back(three);

				indices.push_back(zero);
				indices.push_back(three);
				indices.push_back(two);
			}
		}

		std::vector<glm::vec3> normals(positions.size(), glm::vec3(0));
		for (uint32_t i = 0; i < normals.size(); i++) {
			normals[i] += positions[i];
			glm::normalize(normals[i]);

		}

		std::vector<Vertex> vertices;
		vertices.resize(positions.size());
		for (uint32_t i = 0; i < vertices.size(); i++) {
			vertices[i].position = positions[i];
			vertices[i].texCoord = texCoords[i];
			vertices[i].normal = normals[i];
			vertices[i].color = glm::vec4(1.0f);
		}

		return new Mesh(vertices, indices);
	}

}
