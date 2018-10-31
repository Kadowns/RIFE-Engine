#pragma once

#include <MaterialBuilder.h>
#include <ShaderFactory.h>


namespace Rife::Graphics {

    class MaterialFactory {
    public:

        static Material* defaultMaterial();

        static Material* surfaceMaterial(
            glm::vec4& color,
            float_t specularPower,
            Texture* diffuseTex,
            Texture* specularTex
        );

    };
}
