#pragma once
#include <VkUtilities.h>
#include <Mesh.h>
#include <Renderer.h>

class MeshRenderer : Renderer {

public:

	void draw();
	void createCommandBuffers();
	void freeCommandBuffer();
	void updateTransformInformation(glm::mat4& vp, uint32_t imageIndex, float time);
	MeshRenderer(Mesh *mesh, Entity::Object* father);
	~MeshRenderer();

private:

	void createDescriptorPool();
	void createDescriptorSets();


	Mesh* p_mesh;
	VkBuffer m_vertexBuffer, m_indexBuffer;
	VkDeviceMemory m_vertexBufferMemory, m_indexBufferMemory;
	std::vector<VkBuffer> m_uniformBuffers;
	std::vector<VkDeviceMemory> m_uniformBuffersMemory;
	std::vector<VkDescriptorSet> m_descriptorSets;
	VkDescriptorPool m_descriptorPool;

};