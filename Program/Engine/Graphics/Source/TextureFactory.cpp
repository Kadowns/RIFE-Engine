#include <TextureFactory.h>

#include <VulkanTools.h>
#include <VulkanData.h>

#include <RifePath.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


namespace Rife::Graphics {

    Texture* TextureFactory::loadTexture(const std::string& path) {
        VkImage image;
        VkDeviceMemory memory;
        VkImageView imageView;
        VkSampler sampler;

        createTextureImage(path, image, memory);
        createTextureImageView(image, imageView);
        createTextureSampler(sampler);

        return new Texture(image, imageView, memory, sampler);
    }

    Texture* TextureFactory::defaultTexture() {
        VkImage image;
        VkDeviceMemory memory;
        VkImageView imageView;
        VkSampler sampler;

        createTextureImage("", image, memory);
        createTextureImageView(image, imageView);
        createTextureSampler(sampler);

        return new Texture(image, imageView, memory, sampler);
    }

    void TextureFactory::createTextureImage(const std::string& path, VkImage& image, VkDeviceMemory& memory) {

        int texWidth, texHeight, texChannels;
        VkDeviceSize imageSize;
        stbi_uc* pixels = stbi_load((TEXTURE_FOLDER + path).data(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        

        if (!pixels) {
            pixels = stbi_load((TEXTURE_FOLDER + std::string("default_texture.png")).data(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        }
       
        imageSize = texWidth * texHeight * texChannels;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        VulkanTools::createBuffer(
            imageSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory
        );

        void* data;
        vkMapMemory(VK_DATA->getDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, pixels, static_cast<size_t>(imageSize));
        vkUnmapMemory(VK_DATA->getDevice(), stagingBufferMemory);

        stbi_image_free(pixels);

        VulkanTools::createImage(
            texWidth, texHeight,
            VK_FORMAT_R8G8B8A8_UNORM,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            image,
            memory
        );

        VulkanTools::transitionImageLayout(image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        VulkanTools::copyBufferToImage(stagingBuffer, image, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
        VulkanTools::transitionImageLayout(image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        vkDestroyBuffer(VK_DATA->getDevice(), stagingBuffer, nullptr);
        vkFreeMemory(VK_DATA->getDevice(), stagingBufferMemory, nullptr);
    }

    void TextureFactory::createTextureImageView(VkImage& image, VkImageView& imageView) {
        imageView = VulkanTools::createImageView(image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);
    }

    void TextureFactory::createTextureSampler(VkSampler& sampler) {

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
        samplerInfo.maxLod = 0.0f;

        if (vkCreateSampler(VK_DATA->getDevice(), &samplerInfo, nullptr, &sampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler!");
        }

    }
}
