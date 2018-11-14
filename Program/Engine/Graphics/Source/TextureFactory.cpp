#include <TextureFactory.h>

#include <VulkanTools.h>
#include <VulkanData.h>

#include <RifePath.h>

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>

#include <gli/gli.hpp>

#include <Buffer.h>

namespace Rife::Graphics {

    Texture* TextureFactory::loadTexture(const std::string& path) {
        TextureInfo textureInfo = {};

        createTexture2DImage(path, textureInfo);

        return new Texture(textureInfo);
    }

    Texture* TextureFactory::defaultTexture() {
        TextureInfo textureInfo = {};

        createTexture2DImage("",textureInfo);

        return new Texture(textureInfo);
    }

    Texture* TextureFactory::loadCubemap(const std::string& path) {

        TextureInfo textureInfo = {};

        createCubemapImage(path, textureInfo);

        return new Texture(textureInfo);
    }

    void TextureFactory::createTexture2DImage(const std::string& path, TextureInfo& texture) {

        gli::texture tex(gli::load(TEXTURE_FOLDER + path));

        if (tex.empty()) {
            gli::texture tex = gli::load(TEXTURE_FOLDER + std::string("default_texture.ktx"));
        }
    
        texture.extent.width = tex.extent().x;
        texture.extent.height = tex.extent().y;
        texture.mipLevels = tex.levels();
        texture.layerCount = tex.layers();

		Buffer stagingBuffer;
		BufferInfo info = {};
		info.memoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		info.usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        
		VulkanTools::createBuffer(
			tex.size(),
			info,
			stagingBuffer,
			tex.data()
        );

        VkFormat format;
        VkPhysicalDeviceFeatures deviceFeatures = VK_DATA->getPhysicalDeviceFeatures();
        if (deviceFeatures.textureCompressionBC) {
            format = VK_FORMAT_BC3_UNORM_BLOCK;
        }
        else if (deviceFeatures.textureCompressionASTC_LDR) {
            format = VK_FORMAT_ASTC_8x8_UNORM_BLOCK;
        }
        else if (deviceFeatures.textureCompressionETC2) {
            format = VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;
        }
        else {
            throw std::runtime_error("Failed to find a supported texture format!");
        }


        VulkanTools::createImage(
            texture.extent.width, texture.extent.height, texture.mipLevels, 1,
            format, VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            texture.image,
            texture.memory,
            0
        );       

        VkImageSubresourceRange subresourceRange = {};
        subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        subresourceRange.baseMipLevel = 0;
        subresourceRange.levelCount = 1;
        subresourceRange.baseArrayLayer = 0;
        subresourceRange.layerCount = 1; 

        VulkanTools::transitionImageLayout(texture.image, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, subresourceRange);
        copyBufferToImage(stagingBuffer.buffer, texture.image, texture.extent.width, texture.extent.height);
        VulkanTools::transitionImageLayout(texture.image, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, subresourceRange);

		stagingBuffer.destroy();

        texture.imageView = VulkanTools::createImageView(texture.image, format, VK_IMAGE_VIEW_TYPE_2D, subresourceRange);

        createTextureSampler(texture);
    }

    void TextureFactory::createCubemapImage(const std::string& path, TextureInfo& texture) {
        
        gli::texture_cube texCube(gli::load(TEXTURE_FOLDER + path));

        assert(!texCube.empty());

        texture.extent.width = texCube.extent().x;
        texture.extent.height = texCube.extent().y;
        texture.mipLevels = texCube.levels();
        texture.layerCount = texCube.layers();

		Buffer stagingBuffer;

		BufferInfo info = {};
		info.memoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		info.usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		VulkanTools::createBuffer(
			texCube.size(),
			info,
			stagingBuffer,
			texCube.data()
		);


        VkFormat format;
        VkPhysicalDeviceFeatures deviceFeatures = VK_DATA->getPhysicalDeviceFeatures();
        if (deviceFeatures.textureCompressionBC) {
            format = VK_FORMAT_BC3_UNORM_BLOCK;
        }
        else if (deviceFeatures.textureCompressionASTC_LDR) {
            format = VK_FORMAT_ASTC_8x8_UNORM_BLOCK;
        }
        else if (deviceFeatures.textureCompressionETC2) {
            format = VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;
        }
        else {
            throw std::runtime_error("Failed to find a supported texture format!");
        }


        VulkanTools::createImage(
            texture.extent.width, texture.extent.height, texture.mipLevels, 6,
            format, VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            texture.image,
            texture.memory,
            VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT
        );


        VkImageSubresourceRange subresourceRange = {};
        subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        subresourceRange.baseMipLevel = 0;
        subresourceRange.levelCount = texture.mipLevels;
        subresourceRange.layerCount = 6;
       
        VulkanTools::transitionImageLayout(
            texture.image, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, subresourceRange
        );
        copyBufferToCubemap(stagingBuffer.buffer, texture.image, texCube);
        VulkanTools::transitionImageLayout(
            texture.image, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, subresourceRange
        );
        
		stagingBuffer.destroy();

        texture.imageView = VulkanTools::createImageView(texture.image, format, VK_IMAGE_VIEW_TYPE_CUBE, subresourceRange);

        createTextureSampler(texture);
    }


    void TextureFactory::createTextureSampler(TextureInfo& texture) {

        VkSamplerCreateInfo samplerInfo = {};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = 16;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = texture.mipLevels;

        if (vkCreateSampler(VK_DATA->getDevice(), &samplerInfo, nullptr, &texture.sampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler!");
        }
    }

    void TextureFactory::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {

        VkCommandBuffer commandBuffer = VulkanTools::beginSingleTimeCommands();

        VkBufferImageCopy region = {};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;

        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = {
            width,
            height,
            1
        };

        vkCmdCopyBufferToImage(
            commandBuffer,
            buffer,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region
        );

        VulkanTools::endSingleTimeCommands(commandBuffer);

    }

    void TextureFactory::copyBufferToCubemap(VkBuffer buffer, VkImage image, gli::texture_cube& cube) {

        VkCommandBuffer commandBuffer = VulkanTools::beginSingleTimeCommands();

        std::vector<VkBufferImageCopy> bufferCopyRegions;
        uint32_t offset = 0;

        for (uint32_t face = 0; face < 6; face++) {
            for (uint32_t level = 0; level < cube.levels(); level++) {
                VkBufferImageCopy bufferCopyRegion = {};
                bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                bufferCopyRegion.imageSubresource.mipLevel = level;
                bufferCopyRegion.imageSubresource.baseArrayLayer = face;
                bufferCopyRegion.imageSubresource.layerCount = 1;
                bufferCopyRegion.imageExtent.width = cube[face][level].extent().x;
                bufferCopyRegion.imageExtent.height = cube[face][level].extent().y;
                bufferCopyRegion.imageExtent.depth = 1;
                bufferCopyRegion.bufferOffset = offset;

                bufferCopyRegions.push_back(bufferCopyRegion);

                // Increase offset into staging buffer for next level / face
                offset += cube[face][level].size();
            }
        }

        vkCmdCopyBufferToImage(
            commandBuffer,
            buffer,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            static_cast<uint32_t>(bufferCopyRegions.size()),
            bufferCopyRegions.data()
        );

        VulkanTools::endSingleTimeCommands(commandBuffer);
    }
}
