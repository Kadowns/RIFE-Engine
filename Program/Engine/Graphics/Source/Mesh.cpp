#include "..\Include\Mesh.h"

Mesh::Mesh(std::vector<Graphics::Vertex> vertices, std::vector<uint32_t> indices) {
	m_vertices = vertices;
	m_indices = indices;
}
