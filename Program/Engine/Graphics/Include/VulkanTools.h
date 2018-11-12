#pragma once

#include <VulkanInclude.h>
#include <vector>

namespace Rife::Graphics {

    class VulkanTools final {
    public:

        static VkCommandBuffer beginSingleTimeCommands();

        static void endSingleTimeCommands(VkCommandBuffer commandBuffer);

        static VkDeviceSize createBuffer(
            VkDeviceSize size, VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory
        );

        static void createImage(
            uint32_t width, uint32_t height, uint32_t mipLevels, uint32_t arrayLayers,
            VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
            VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, const VkImageCreateFlags& flags
        );

        static VkImageView createImageView(VkImage image, VkFormat format, VkImageViewType viewType, VkImageSubresourceRange subresourceRange);

        static void transitionImageLayout(
            VkImage image, VkFormat format,
            VkImageLayout oldLayout, VkImageLayout newLayout,
            VkImageSubresourceRange subresourceRange
        );

		static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDeviceSize offset);

        static VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

        static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        static bool hasStencilComponent(VkFormat format);

    };


}