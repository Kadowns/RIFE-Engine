#include <GraphicsPipeline.h>

void gph::GraphicsPipeline::createGraphicsPipeline() {
    gph::Shader::bindDevice(new VkDevice())
        .bindShader("a", VK_SHADER_STAGE_VERTEX_BIT)
        .bindShader("b", VK_SHADER_STAGE_FRAGMENT_BIT)
        .unbindDevice();
}