#pragma once
#include <VkUtilities.h>
#include <Vertex.h>
#include <vector>

namespace Rife::Graphics {
    class Mesh {

    public:
        Mesh() {}
        ~Mesh();
        Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
        std::vector<Vertex>& getVertices() { return m_vertices; }
        std::vector<uint32_t>& getIndices() { return m_indices; }

        VkBuffer& getVertexBuffer() { return m_vertexBuffer; }
        VkBuffer& getIndexBuffer() { return m_indexBuffer; }

        VkDeviceMemory& getVertexBufferMemory() { return m_vertexBufferMemory; }
        VkDeviceMemory& getIndexBufferMemory() { return m_indexBufferMemory; }

    private:

        std::vector<Vertex> m_vertices;
        std::vector<uint32_t> m_indices;

        VkBuffer m_vertexBuffer, m_indexBuffer;
        VkDeviceMemory m_vertexBufferMemory, m_indexBufferMemory;

    };
}