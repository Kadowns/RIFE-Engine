#include <GraphicsDependencys.h>
#include <VkUtilities.h>
#include <Mesh.h>
#include <Renderer.h>

class MeshRenderer : Renderer {
	void draw();
	void recordCmdBuffer();
public:
	MeshRenderer(Mesh *mesh);
private:
    Mesh* p_mesh;
    VkBuffer m_vertexBuffer, m_indicesBuffer, m_uniformBuffer;
    VkDeviceMemory m_vertexBufferMemory, m_indicesBufferMemory, m_uniformBufferMemory;
};