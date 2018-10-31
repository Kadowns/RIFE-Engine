#include <Mesh.h>
namespace Rife::Graphics {

    Mesh::~Mesh() {

        vkDestroyBuffer(VK_WRAPPER->getDevice(), m_vertexBuffer, nullptr);
        vkFreeMemory(VK_WRAPPER->getDevice(), m_vertexBufferMemory, nullptr);

        vkDestroyBuffer(VK_WRAPPER->getDevice(), m_indexBuffer, nullptr);
        vkFreeMemory(VK_WRAPPER->getDevice(), m_indexBufferMemory, nullptr);
    }

    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices) {
        m_vertices = vertices;
        m_indices = indices;

        VkDeviceSize bufferSize = sizeof(m_vertices[0]) * m_vertices.size();
        VK_WRAPPER->createVertexBuffer(m_vertexBuffer, m_vertexBufferMemory, bufferSize, m_vertices.data());

        bufferSize = sizeof(m_indices[0]) * m_indices.size();
        VK_WRAPPER->createIndexBuffer(m_indexBuffer, m_indexBufferMemory, bufferSize, m_indices.data());
    }

}
