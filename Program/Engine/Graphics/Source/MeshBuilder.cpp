#include <MeshBuilder.h>

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
				for (uint8_t j = 0; j < att.second; j++) {
					vertices[i] = std::move(att.first[j + vert * att.second]);
					i++;
				}
			}
		}

		return new Mesh(vertices, m_indices);
	}
}