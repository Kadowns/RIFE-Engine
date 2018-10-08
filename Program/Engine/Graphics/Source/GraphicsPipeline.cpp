#include <GraphicsPipeline.h>

void gph::GraphicsPipeline::createGraphicsPipeline() {
    gph::ShaderModule* a = new gph::ShaderModule();
    a->bindDevice(new VkDevice())
        ->bindShader("a", VK_SHADER_STAGE_VERTEX_BIT)
        ->bindShader("b", VK_SHADER_STAGE_FRAGMENT_BIT)
        ->unbindDevice();
    
    delete a;
}