#include <Vertex.h>

namespace Rife::Graphics {

    size_t VertexLayout::stride() {
        size_t stride = 0;
        for (auto comp : m_components) {
            stride += size(comp);
        }
        return stride;
    }

    VkVertexInputBindingDescription VertexLayout::getBindingDescription() {

        VkVertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = stride();
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    std::vector<VkVertexInputAttributeDescription> VertexLayout::getAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
        attributeDescriptions.resize(m_components.size());
        uint32_t offset = 0;
        for (uint8_t i = 0; i < m_components.size(); i++) {
            attributeDescriptions[i].binding = 0;
            attributeDescriptions[i].location = i;
            attributeDescriptions[i].format = componentFormat(m_components[i]);
            attributeDescriptions[i].offset = offset;
            offset += size(m_components[i]);
        }

        return attributeDescriptions;
    }

    VkFormat VertexLayout::componentFormat(const VERTEX_COMPONENT& component) {
        switch (component) {
        case VERTEX_COMPONENT_UV:
            return VK_FORMAT_R32G32_SFLOAT;
        case VERTEX_COMPONENT_COLOR:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        default:
            return VK_FORMAT_R32G32B32_SFLOAT;
        }
    }

    uint32_t VertexLayout::size(const VERTEX_COMPONENT& component) {
        switch (component) {
        case VERTEX_COMPONENT_UV:
            return sizeof(float) * 2;
        case VERTEX_COMPONENT_COLOR:
            return sizeof(float) * 4;
        default:
            return sizeof(float) * 3;
        }
    }
    
}