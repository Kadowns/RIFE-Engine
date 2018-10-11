#pragma once
#include <GraphicsDependencys.h>
#include <Vertex.h>
#include <vector>

class Mesh {

public:
	Mesh(std::vector<gph::Vertex> vertices, std::vector<uint32_t> indices);
	std::vector<gph::Vertex>& getVertices() { return m_vertices; }
	std::vector<uint32_t>& getIndices() { return m_indices; }

private:
	
	std::vector<gph::Vertex> m_vertices;
	std::vector<uint32_t> m_indices;

};