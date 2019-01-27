#include <MaterialFactory.h>

namespace Rife::Graphics {

    Material* MaterialFactory::defaultMaterial() {

        return MaterialBuilder()
            .setShader(ShaderFactory::defaultShader())
            .createMaterial();
    }

    Material* MaterialFactory::skyboxMaterial(Texture* cubemap) {

        return MaterialBuilder()
            .setShader(ShaderFactory::skyboxShader())
            .addTexture(cubemap, 0)
            .createMaterial();
    }

    Material* MaterialFactory::surfaceMaterial(
        Texture* diffuseTex,
        Texture* specularTex,
		Texture* reflectionCube
    ) {
        
        return MaterialBuilder()
            .setShader(ShaderFactory::surfaceShader("surface_vert.spv", "surface_frag.spv"))
            .addTexture(reflectionCube, 2)
            .addTexture(specularTex, 1)
            .addTexture(diffuseTex, 0)
            .createMaterial();
    }

    Material* MaterialFactory::terrainMaterial(Texture* tex1, Texture* tex2, Texture* tex3, Texture* tex4) {
        return MaterialBuilder()
            .setShader(ShaderFactory::terrainShader())
            .addTexture(tex4, 3)
            .addTexture(tex3, 2)
            .addTexture(tex2, 1)
            .addTexture(tex1, 0)
            .createMaterial();
    }
}


