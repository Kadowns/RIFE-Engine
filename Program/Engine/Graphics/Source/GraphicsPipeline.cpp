#include <GraphicsPipeline.h>

void Graphics::GraphicsPipeline::createGraphicsPipeline() {
    Graphics::ShaderModule* a = new Graphics::ShaderModule();
    a->bindDevice(new VkDevice())
        ->bindShader("a", VK_SHADER_STAGE_VERTEX_BIT)
        ->bindShader("b", VK_SHADER_STAGE_FRAGMENT_BIT)
        ->unbindDevice();
    
    delete a;
}