#include <MaterialFactory.h>

namespace Rife::Graphics {

    Material* MaterialFactory::defaultMaterial() {

		auto shader = ShaderFactory::defaultShader("default_vert.spv", "default_frag.spv");

		Ubo::uMaterialProperties properties = {};
        properties.color = glm::vec4(1.0f);
        properties.specularPower = 16.0f;

		auto material = MaterialBuilder()
	        .setShader(shader)
			.setMaterialProperties(properties)
            .createMaterial();
       
        return material;
    }

    Material* MaterialFactory::surfaceMaterial(
        glm::vec4& color,
        float_t specularPower,
        Texture* diffuseTex,
        Texture* specularTex
    ) {
        
        auto shader = ShaderFactory::surfaceShader("surface_vert.spv", "surface_frag.spv");

        Ubo::uMaterialProperties properties = {};
        properties.color = color;
        properties.specularPower = specularPower;

        auto material = MaterialBuilder()
            .setShader(shader)
            .setMaterialProperties(properties)
            .addTexture(specularTex, MATERIAL_TEXTURE_TYPE_SPECULAR_MAP)
            .addTexture(diffuseTex, MATERIAL_TEXTURE_TYPE_DIFFUSE_MAP)
            .createMaterial();

        return material;
    }
}


