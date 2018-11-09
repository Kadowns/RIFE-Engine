#pragma once

#include <RifeMath.h>
#include <VulkanInclude.h>
#include <array>

namespace Rife::Graphics {

	enum VERTEX_COMPONENT {
		VERTEX_COMPONENT_POSITION = 0x0,
		VERTEX_COMPONENT_COLOR = 0x1,
		VERTEX_COMPONENT_NORMAL = 0x2,
		VERTEX_COMPONENT_UV = 0x3
	};

	struct VertexLayout {

		std::vector<VERTEX_COMPONENT> m_vertexComponents;

		VertexLayout(std::vector<VERTEX_COMPONENT> components) {
			m_vertexComponents = std::move(components);
		}
	};

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;

        static VkVertexInputBindingDescription getBindingDescription();
        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();

        bool operator==(const Vertex& other) const {
            return position == other.position && normal == other.normal && texCoord == other.texCoord;
        }
    };
}
namespace std {
    template<> struct hash<Rife::Graphics::Vertex> {
        size_t operator()(Rife::Graphics::Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.position) ^
                (hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^
                (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

