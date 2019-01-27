#include <Texture.h>
#include <VulkanData.h>

namespace Rife::Graphics {

    Texture::Texture(const TextureInfo& textureInfo) {
        m_name = "Texture";
        m_info = std::move(textureInfo);
    }

    Texture::~Texture() {
        vkDestroySampler(Vulkan::device, m_info.sampler, nullptr);

        vkDestroyImageView(Vulkan::device, m_info.imageView, nullptr);

        vkDestroyImage(Vulkan::device, m_info.image, nullptr);
        vkFreeMemory(Vulkan::device, m_info.memory, nullptr);
    }
}

