#pragma once
#include <GraphicsDependencys.h>
#include <UniformBufferObject.h>
#include <Component.h>

namespace Rife::Graphics {
    class Renderer : public Base::Component {
    public:
        virtual void draw() = 0;
        virtual void createCommandBuffers() = 0;
        virtual void updateTransformInformation(glm::mat4& vp, uint32_t imageIndex, float time) = 0;
    protected:

        Ubo::Mvp m_ubo;
        std::vector<VkCommandBuffer> m_commandBuffers;

    };
}
