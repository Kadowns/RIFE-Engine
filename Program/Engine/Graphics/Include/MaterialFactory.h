#pragma once

#include <MaterialBuilder.h>
#include <ShaderFactory.h>


namespace Rife::Graphics {

    class MaterialFactory {
    public:

        static Material* defaultMaterial();

        static Material* skyboxMaterial(Texture* cubemap);

        static Material* surfaceMaterial(
            Texture* diffuseTex,
            Texture* specularTex
        );

    };
}
