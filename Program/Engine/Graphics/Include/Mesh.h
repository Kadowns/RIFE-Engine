#pragma once
#include <GraphicsLibs.h>
#include <Vertex.h>
#include <vector>

namespace Rife::Graphics {
    class Mesh {

    public:
        Mesh() {}
        Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
        std::vector<Vertex>& getVertices() { return m_vertices; }
        std::vector<uint32_t>& getIndices() { return m_indices; }

    private:

        std::vector<Vertex> m_vertices;
        std::vector<uint32_t> m_indices;

    };
}