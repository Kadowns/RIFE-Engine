#pragma once

#include <RifeCore.h>
#include <VulkanInclude.h>

namespace Rife::Graphics {

    struct TextureInfo {

        VkImage image;
        VkImageView imageView;
        VkSampler sampler;
        VkDeviceMemory memory;

        struct {

            uint32_t width, height;

        } extent;

        uint32_t mipLevels, layerCount;
    };


    class Texture final : public Base::Object {

    public:

        Texture(const TextureInfo& textureInfo);
        ~Texture();

        VkImage& getImage() { return m_info.image; }
        VkImageView& getImageView() { return m_info.imageView; }
        VkSampler& getSampler() { return m_info.sampler; }
        VkDeviceMemory& getMemory() { return m_info.memory; }

        void serialize(std::ofstream& file) {
            file << (m_name + "\n").c_str();
        }

    private:

        TextureInfo m_info;
    };
}
