#pragma once

#include <VkUtilities.h>

namespace Rife::Graphics {

    class Texture final : public Base::Object {

    public:

        Texture(const std::string& path);
        ~Texture();

        VkImage& getImage() { return m_textureImage; }
        VkImageView& getImageView() { return m_textureImageView; }
        VkSampler& getSampler() { return m_textureSampler; }
        VkDeviceMemory& getMemory() { return m_textureImageMemory; }

    private:

        VkImage m_textureImage;
        VkImageView m_textureImageView;
        VkSampler m_textureSampler;
        VkDeviceMemory m_textureImageMemory;

    };
}
