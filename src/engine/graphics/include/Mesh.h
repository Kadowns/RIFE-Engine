#pragma once
#include <VulkanInclude.h>
#include <Vertex.h>
#include <Buffer.h>
#include <vector>

namespace Rife::Graphics {

    class Mesh {

    public:
        Mesh() {}
        ~Mesh();
        Mesh(std::vector<float> vertices, std::vector<uint32_t> indices);
        std::vector<float>& getVertices() { return m_vertices; }
        std::vector<uint32_t>& getIndices() { return m_indices; }

        Buffer& getVertexBuffer() { return m_vertexBuffer; }
        Buffer& getIndexBuffer() { return m_indexBuffer; }

        void serialize(std::ofstream&) {

        }

    private:

		void createVertexBuffer(Buffer& buffer, VkDeviceSize bufferSize, void* verticesData);
		void createIndexBuffer(Buffer& buffer, VkDeviceSize bufferSize, void* indicesData);

        std::vector<float> m_vertices;
		uint32_t m_verticesCount = 0;
        std::vector<uint32_t> m_indices;
        Buffer m_vertexBuffer, m_indexBuffer;

    };
}