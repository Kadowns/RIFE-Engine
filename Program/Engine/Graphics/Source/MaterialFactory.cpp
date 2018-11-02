#include <MaterialFactory.h>

namespace Rife::Graphics {

    Material* MaterialFactory::defaultMaterial() {

		auto shader = ShaderFactory::defaultShader("default_vert.spv", "default_frag.spv");

		auto material = MaterialBuilder()
	        .setShader(shader)
            .createMaterial();
       
        return material;
    }

    Material* MaterialFactory::surfaceMaterial(
        Texture* diffuseTex,
        Texture* specularTex
    ) {
        
        auto shader = ShaderFactory::surfaceShader("surface_vert.spv", "surface_frag.spv");


        auto material = MaterialBuilder()
            .setShader(shader)
            .addTexture(specularTex, MATERIAL_TEXTURE_TYPE_SPECULAR_MAP)
            .addTexture(diffuseTex, MATERIAL_TEXTURE_TYPE_DIFFUSE_MAP)
            .createMaterial();

        return material;
    }
}


