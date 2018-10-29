#include <MaterialFactory.h>

namespace Rife::Graphics {

    Material* MaterialFactory::defaultMaterial(const std::string& vertShaderName, const std::string& fragShaderName) {

		auto shader = ShaderFactory::defaultShader(vertShaderName, fragShaderName);

		Ubo::uMaterialProperties properties = {};
		properties.ambient = glm::vec4(0.5f, 0.0f, 0.5f, 0.0f);
		properties.diffuse = glm::vec4(0.5f, 0.0f, 0.5f, 0.0f);
		properties.specular = glm::vec4(1.0f, 1.0f, 1.0f, 64.0f);

		auto material = MaterialBuilder()
	        .setShader(shader)
			.setMaterialProperties(properties)
            .createMaterial();

       
        return material;
    }
}


