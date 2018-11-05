#pragma once

#include <RifeMath.h>
#include <VulkanInclude.h>
#include <array>

namespace Rife::Graphics {

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

