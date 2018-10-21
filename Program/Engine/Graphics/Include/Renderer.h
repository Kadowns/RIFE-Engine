#pragma once
#include <GraphicsDependencys.h>
#include <UniformBufferObject.h>
#include <Component.h>

namespace Rife::Graphics {
    class Renderer : public Base::Component {
    public:
        virtual void draw() = 0;
        virtual void createCommandBuffers() = 0;
        virtual void updateTransformInformation(const glm::mat4& vp, const uint32_t& imageIndex) = 0;
        virtual void freeCommandBuffers() = 0;
    protected:

        Ubo::Mvp m_ubo;
        std::vector<VkCommandBuffer> m_commandBuffers;

    };
}
