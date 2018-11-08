#include <Mesh.h>
#include <VulkanTools.h>
#include <VulkanData.h>

namespace Rife::Graphics {

    Mesh::~Mesh() {

        vkDestroyBuffer(VK_DATA->getDevice(), m_vertexBuffer, nullptr);
        vkFreeMemory(VK_DATA->getDevice(), m_vertexBufferMemory, nullptr);

        vkDestroyBuffer(VK_DATA->getDevice(), m_indexBuffer, nullptr);
        vkFreeMemory(VK_DATA->getDevice(), m_indexBufferMemory, nullptr);
    }

    Mesh::Mesh(std::vector<float> vertices, std::vector<uint32_t> indices) {
        m_vertices = vertices;
        m_indices = indices;

        VkDeviceSize bufferSize = sizeof(m_vertices[0]) * m_vertices.size();
		createVertexBuffer(m_vertexBuffer, m_vertexBufferMemory, bufferSize, m_vertices.data());

        bufferSize = sizeof(m_indices[0]) * m_indices.size();
		createIndexBuffer(m_indexBuffer, m_indexBufferMemory, bufferSize, m_indices.data());
    }

	void Mesh::createVertexBuffer(
		VkBuffer& buffer,
		VkDeviceMemory& memory,
		VkDeviceSize bufferSize,
		void* verticesData
	){

		VulkanTools::createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			buffer,
			memory
		);

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		VulkanTools::createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer,
			stagingBufferMemory
		);

		void* data;
		vkMapMemory(VK_DATA->getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, verticesData, (size_t)bufferSize);//copia os dados da memoria dos vertices pra gpu
		vkUnmapMemory(VK_DATA->getDevice(), stagingBufferMemory);

		VulkanTools::copyBuffer(stagingBuffer, buffer, bufferSize, 0);

		vkDestroyBuffer(VK_DATA->getDevice(), stagingBuffer, nullptr);
		vkFreeMemory(VK_DATA->getDevice(), stagingBufferMemory, nullptr);

	}

	void Mesh::createIndexBuffer(
		VkBuffer& buffer,
		VkDeviceMemory& memory,
		VkDeviceSize bufferSize,
		void* indicesData
	) {
		VulkanTools::createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			buffer,
			memory
		);


		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		VulkanTools::createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer,
			stagingBufferMemory
		);

		void* data;
		vkMapMemory(VK_DATA->getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, indicesData, (size_t)bufferSize);
		vkUnmapMemory(VK_DATA->getDevice(), stagingBufferMemory);

		VulkanTools::copyBuffer(stagingBuffer, buffer, bufferSize, 0);

		vkDestroyBuffer(VK_DATA->getDevice(), stagingBuffer, nullptr);
		vkFreeMemory(VK_DATA->getDevice(), stagingBufferMemory, nullptr);
	}

}
