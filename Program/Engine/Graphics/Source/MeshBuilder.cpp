#include <MeshBuilder.h>
#include <utility>

namespace Rife::Graphics {

	MeshBuilder& MeshBuilder::setIndices(std::vector<uint32_t> indices){
		m_indices = indices;
		return *this;
	}


	Mesh* MeshBuilder::create(const uint32_t& verticesCount) {
		
		uint32_t attSize = 0;
		for (auto att : m_attributes) {
			attSize += att.second;
		}

		std::vector<float> vertices;
		vertices.resize(verticesCount * attSize);
		uint32_t i = 0;
		for (uint32_t vert = 0; vert < verticesCount; vert++) {
			for (auto att : m_attributes) {
                //é mais ou menos 1 segundo mais rapido do que um for quando carrega um modelo com uns 70 mil vertices
                switch (att.second) {
                case 1:
                    vertices[i] = std::move(att.first[vert]);
                    i++;
                    break;
                case 2:
                    vertices[i] = std::move(att.first[vert * att.second]);
                    vertices[i + 1] = std::move(att.first[1 + vert * att.second]);
                    i += 2;
                    break;
                case 3:
                    vertices[i] = std::move(att.first[vert * att.second]);
                    vertices[i + 1] = std::move(att.first[1 + vert * att.second]);
                    vertices[i + 2] = std::move(att.first[2 + vert * att.second]);
                    i += 3;
                    break;
                case 4:
                    vertices[i] = std::move(att.first[vert * att.second]);
                    vertices[i + 1] = std::move(att.first[1 + vert * att.second]);
                    vertices[i + 2] = std::move(att.first[2 + vert * att.second]);
                    vertices[i + 3] = std::move(att.first[3 + vert * att.second]);
                    i += 4;
                    break;
                }
				/*for (uint8_t j = 0; j < att.second; j++) {
					vertices[i] = std::move(att.first[j + vert * att.second]);
					i++;
				}*/
			}
		}

		return new Mesh(vertices, m_indices);
	}
}