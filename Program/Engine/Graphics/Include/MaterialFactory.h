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
            Texture* specularTex,
			Texture* reflectionCube
        );

        static Material* terrainMaterial(Texture* tex1, Texture* tex2, Texture* tex3, Texture* tex4);

    };
}
