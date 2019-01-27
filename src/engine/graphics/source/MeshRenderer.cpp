#include <VulkanData.h>
#include <VulkanBase.h>

#include <MeshRenderer.h>
#include <MaterialFactory.h>
#include <Transform.h>
#include <Camera.h>
#include <GlobalLights.h>

namespace Rife::Graphics {


    MeshRenderer::MeshRenderer(Mesh* mesh, const MaterialInstance& material) {
        m_name = "MeshRenderer";
        p_mesh = mesh;
        m_material = material;
    }

    void MeshRenderer::createCommandBuffers() {
        m_commandBuffers.resize(Vulkan::swapchainImages.size());

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = Vulkan::commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
        allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();

        if (vkAllocateCommandBuffers(Vulkan::device, &allocInfo, m_commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }

        for (int i = 0; i < m_commandBuffers.size(); i++) {

            VkCommandBufferInheritanceInfo inheritanceInfo = {};
            inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
            inheritanceInfo.pNext = nullptr;
            inheritanceInfo.renderPass = Vulkan::renderPass;
            inheritanceInfo.framebuffer = Vulkan::framebuffers[i];
            inheritanceInfo.subpass = 0;


            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
            beginInfo.pInheritanceInfo = &inheritanceInfo;

            if (vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("failed to begin recording command buffer!");
            }

            vkCmdPushConstants(
                m_commandBuffers[i],
                *m_material.getShader()->getPipelineLayout(),
                VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(Ubo::MaterialProperties),
                (void*)&m_material.getProperties()
            );

            vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *m_material.getShader()->getPipeline());

            VkBuffer vertexBuffers[] = { p_mesh->getVertexBuffer().buffer };
            VkDeviceSize offsets[] = { 0 };
            vkCmdBindVertexBuffers(m_commandBuffers[i], 0, 1, vertexBuffers, offsets);

            vkCmdBindIndexBuffer(m_commandBuffers[i], p_mesh->getIndexBuffer().buffer, 0, VK_INDEX_TYPE_UINT32);

            vkCmdBindDescriptorSets(
                m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
                *m_material.getShader()->getPipelineLayout(), 0, 1, &m_descriptorSets[i], 0, nullptr
            );

            vkCmdDrawIndexed(m_commandBuffers[i], p_mesh->getIndices().size(), 1, 0, 0, 0);

            if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer!");
            }
        }
        VK_BASE->bindCommandBuffer(&m_commandBuffers);
    }
}
