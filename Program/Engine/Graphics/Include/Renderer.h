#pragma once
#include <GraphicsLibs.h>
#include <UniformBufferObject.h>
#include <RifeCore.h>

namespace Rife::Graphics {
    class Renderer : public Base::Component {
    public:
        virtual void draw() = 0;
        virtual void createCommandBuffers() = 0;
        virtual void updateTransformInformation(const glm::mat4& vp,const glm::vec3& cameraPos, const uint32_t& imageIndex) = 0;
        virtual void freeCommandBuffers() = 0;
    protected:

        Ubo::Transform m_ubo;
        std::vector<VkCommandBuffer> m_commandBuffers;

    };
}
