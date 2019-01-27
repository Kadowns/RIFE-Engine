#pragma once

#include <Mesh.h>
#include <map>

namespace Rife::Graphics {

	typedef std::pair<std::vector<float>, int> attribute;


	class MeshBuilder {

	public:

		MeshBuilder& setIndices(std::vector<uint32_t> indices);

		template<int L, typename T, glm::qualifier Q>
		MeshBuilder& addVecAttribute(std::vector<glm::vec<L, T, Q>> vec);

		Mesh* create(const uint32_t& verticesCount);

	private:

		std::vector<attribute> m_attributes;
		std::vector<uint32_t> m_indices;
	};

	template<int L, typename T, glm::qualifier Q>
	inline MeshBuilder& MeshBuilder::addVecAttribute(std::vector<glm::vec<L, T, Q>> vec) {
		if (vec.size() == 0)
			return *this;

		attribute att;
		att.second = L;
		for (auto &v : vec) {
			for (uint8_t i = 0; i < v.length(); i++) {
				att.first.push_back(v[i]);
			}
		}
		m_attributes.push_back(std::move(att));
		return *this;
	}
}