#include <Texture.h>
#include <VulkanData.h>

namespace Rife::Graphics {

    Texture::Texture(VkImage& image, VkImageView& imageView, VkDeviceMemory& memory, VkSampler& sampler) {
        m_textureImage = image;
        m_textureImageView = imageView;
        m_textureImageMemory = memory;
        m_textureSampler = sampler;

    }

    Texture::~Texture() {
        vkDestroySampler(VK_DATA->getDevice(), m_textureSampler, nullptr);

        vkDestroyImageView(VK_DATA->getDevice(), m_textureImageView, nullptr);

        vkDestroyImage(VK_DATA->getDevice(), m_textureImage, nullptr);
        vkFreeMemory(VK_DATA->getDevice(), m_textureImageMemory, nullptr);
    }
}

