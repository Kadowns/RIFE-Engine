#pragma once

#include <Texture.h>

namespace Rife::Graphics {

    class TextureFactory {
    public:
        static Texture* loadTexture(const std::string& path);
        static Texture* defaultTexture();

    private:
        static void createTextureImage(const std::string& path, VkImage& image, VkDeviceMemory& memory);
        static void createTextureImageView(VkImage& image, VkImageView& imageView);
        static void createTextureSampler(VkSampler& sampler);
    };


}