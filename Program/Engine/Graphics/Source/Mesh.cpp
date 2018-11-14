#include <Mesh.h>
#include <VulkanTools.h>
#include <VulkanData.h>

namespace Rife::Graphics {

    Mesh::~Mesh() {

        m_vertexBuffer.destroy();
        m_indexBuffer.destroy();
    }

    Mesh::Mesh(std::vector<float> vertices, std::vector<uint32_t> indices) {
        m_vertices = vertices;
        m_indices = indices;

        VkDeviceSize bufferSize = sizeof(m_vertices[0]) * m_vertices.size();
		createVertexBuffer(m_vertexBuffer, bufferSize, m_vertices.data());

        bufferSize = sizeof(m_indices[0]) * m_indices.size();
		createIndexBuffer(m_indexBuffer, bufferSize, m_indices.data());
    }

	void Mesh::createVertexBuffer(
		Buffer& buffer,
		VkDeviceSize bufferSize,
		void* verticesData
	){

        BufferInfo info = {};
        info.memoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        info.usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

        Buffer stagingBuffer;
        VulkanTools::createBuffer(
            bufferSize,
            info,
            stagingBuffer,
            verticesData
        );


        info.memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        info.usageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

        VulkanTools::createBuffer(
            bufferSize,
            info,
            buffer
        );

        VulkanTools::copyBuffer(stagingBuffer.buffer, buffer.buffer, bufferSize, 0);

        stagingBuffer.destroy();

	}

	void Mesh::createIndexBuffer(
		Buffer& buffer,
		VkDeviceSize bufferSize,
		void* indicesData
	) {
		
        BufferInfo info = {};
        info.memoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        info.usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

        Buffer stagingBuffer;
		VulkanTools::createBuffer(
			bufferSize,
			info, 
            stagingBuffer,
            indicesData
		);
        

        info.memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        info.usageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

        VulkanTools::createBuffer(
            bufferSize,
            info,
            buffer
        );

		VulkanTools::copyBuffer(stagingBuffer.buffer, buffer.buffer, bufferSize, 0);

        stagingBuffer.destroy();
	}

}
