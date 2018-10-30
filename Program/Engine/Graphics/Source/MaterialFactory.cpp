#include <MaterialFactory.h>

namespace Rife::Graphics {

    Material* MaterialFactory::defaultMaterial(const std::string& vertShaderName, const std::string& fragShaderName, Texture* texture) {

		auto shader = ShaderFactory::defaultShader(vertShaderName, fragShaderName);

		Ubo::uMaterialProperties properties = {};
		properties.ambient = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
		properties.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
		properties.specular = glm::vec4(1.0f, 1.0f, 1.0f, 64.0f);

		auto material = MaterialBuilder()
	        .setShader(shader)
			.setMaterialProperties(properties)
            .setDiffuseTexture(texture)
            .createMaterial();

       
        return material;
    }
    Material * MaterialFactory::specularMaterial(const std::string & vertShaderName, const std::string & fragShaderName, Texture * diffuseTex, Texture * specularTex) {
        
        auto shader = ShaderFactory::defaultShader(vertShaderName, fragShaderName);

        Ubo::uMaterialProperties properties = {};
        properties.ambient = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
        properties.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
        properties.specular = glm::vec4(1.0f, 1.0f, 1.0f, 16.0f);

        auto material = MaterialBuilder()
            .setShader(shader)
            .setMaterialProperties(properties)
            .setDiffuseTexture(diffuseTex)
            .setSpecularTexture(specularTex)
            .createMaterial();


        return material;
    }
}


