#include <Texture.h>
#include <VulkanData.h>

namespace Rife::Graphics {

    Texture::Texture(const TextureInfo& textureInfo) {
        m_name = "Texture";
        m_info = textureInfo;
    }

    Texture::~Texture() {
        vkDestroySampler(VK_DATA->getDevice(), m_info.sampler, nullptr);

        vkDestroyImageView(VK_DATA->getDevice(), m_info.imageView, nullptr);

        vkDestroyImage(VK_DATA->getDevice(), m_info.image, nullptr);
        vkFreeMemory(VK_DATA->getDevice(), m_info.memory, nullptr);
    }
}

