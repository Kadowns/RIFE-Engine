#pragma once

#include <Texture.h>

namespace gli {
    class texture_cube;
}

namespace Rife::Graphics {

    class TextureFactory {
    public:
        static Texture* loadTexture(const std::string& path);
        static Texture* loadCubemap(const std::string& path);
        static Texture* defaultTexture();

    private:
        static void createTexture2DImage(const std::string& path, TextureInfo& texture);
        static void createCubemapImage(const std::string& path, TextureInfo& texture);
        static void createTextureSampler(TextureInfo& texture);


        static void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        static void copyBufferToCubemap(VkBuffer buffer, VkImage image, gli::texture_cube& cube);

    };


}