#include <Texture.h>

namespace Rife::Graphics {

    Texture::Texture(const std::string& path) {

        VK_WRAPPER->createTextureImage(path, m_textureImage, m_textureImageMemory);
        VK_WRAPPER->createTextureImageView(m_textureImage, m_textureImageView);
        VK_WRAPPER->createTextureSampler(m_textureSampler);

    }

    Texture::~Texture() {
        vkDestroySampler(*VK_WRAPPER->getDevice(), m_textureSampler, nullptr);

        vkDestroyImageView(*VK_WRAPPER->getDevice(), m_textureImageView, nullptr);

        vkDestroyImage(*VK_WRAPPER->getDevice(), m_textureImage, nullptr);
        vkFreeMemory(*VK_WRAPPER->getDevice(), m_textureImageMemory, nullptr);
    }
}

