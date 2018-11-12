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
            .addTexture(cubemap, MATERIAL_TEXTURE_TYPE_DIFFUSE_MAP)
            .createMaterial();
    }

    Material* MaterialFactory::surfaceMaterial(
        Texture* diffuseTex,
        Texture* specularTex
    ) {
        
        auto shader = ShaderFactory::surfaceShader("surface_vert.spv", "surface_frag.spv");

        return MaterialBuilder()
            .setShader(shader)
            .addTexture(specularTex, MATERIAL_TEXTURE_TYPE_SPECULAR_MAP)
            .addTexture(diffuseTex, MATERIAL_TEXTURE_TYPE_DIFFUSE_MAP)
            .createMaterial();
    }
}


