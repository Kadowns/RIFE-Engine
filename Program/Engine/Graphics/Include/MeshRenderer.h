#pragma once
#include <GraphicsDependencys.h>
#include <VkUtilities.h>
#include <Transform.h>
#include <Mesh.h>
#include <Renderer.h>

class MeshRenderer : Renderer {

public:

	void draw();
	void recordCmdBuffer();
	void setMVP(glm::mat4& vp);
	MeshRenderer(Mesh *mesh, Entity::Object* father);

private:

	Mesh* p_mesh;
	VkBuffer m_vertexBuffer, m_indexBuffer, m_uniformBuffer;
	VkDeviceMemory m_vertexBufferMemory, m_indexBufferMemory, m_uniformBufferMemory;

};