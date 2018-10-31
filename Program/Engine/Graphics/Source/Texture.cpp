#include <Texture.h>

namespace Rife::Graphics {

    Texture::Texture(VkImage& image, VkImageView& imageView, VkDeviceMemory& memory, VkSampler& sampler) {
        m_textureImage = image;
        m_textureImageView = imageView;
        m_textureImageMemory = memory;
        m_textureSampler = sampler;

    }

    Texture::~Texture() {
        vkDestroySampler(VK_WRAPPER->getDevice(), m_textureSampler, nullptr);

        vkDestroyImageView(VK_WRAPPER->getDevice(), m_textureImageView, nullptr);

        vkDestroyImage(VK_WRAPPER->getDevice(), m_textureImage, nullptr);
        vkFreeMemory(VK_WRAPPER->getDevice(), m_textureImageMemory, nullptr);
    }
}

