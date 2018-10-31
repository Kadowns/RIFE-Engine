#include <ShaderFactory.h>
#include <string_view>

namespace Rife::Graphics {

    Shader* ShaderFactory::defaultShader(const std::string& vertShaderName, const std::string& fragShaderName) {
        //Layout bindings
        std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
        //model
        layoutBindings.push_back(createDescriptorSetLayoutBinding(
            0,									//binding
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,	//binding type
            1,									//descriptor count
            VK_SHADER_STAGE_VERTEX_BIT,			//shader stage
            nullptr								//immutabble samplers
        ));

        //camera
        layoutBindings.push_back(createDescriptorSetLayoutBinding(
            1,									//binding
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,	//binding type
            1,									//descriptor count
            VK_SHADER_STAGE_VERTEX_BIT,			//shader stage
            nullptr								//immutabble samplers
        ));

        //light
        layoutBindings.push_back(createDescriptorSetLayoutBinding(
            2,									//binding
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,	//binding type
            1,									//descriptor count
            VK_SHADER_STAGE_FRAGMENT_BIT,		//shader stage
            nullptr								//immutabble samplers
        ));

        VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo = {};
        descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
        descriptorSetLayoutInfo.pBindings = layoutBindings.data();
        //----------------------------------------------------------------------------

        //UniformBufferObjectInfo-------------
        UniformBufferObjectInfo modelInfo = createUboInfo(Ubo::uTransform::size());
        UniformBufferObjectInfo cameraInfo = createUboInfo(Ubo::uCamera::size());
        UniformBufferObjectInfo lightInfo = createUboInfo(Ubo::uLight::size());

        //pushConstant
        VkPushConstantRange materialPushConstant = createPushConstantRange(
            VK_SHADER_STAGE_FRAGMENT_BIT,
            sizeof(Ubo::uMaterialProperties),
            0
        );

        //----------------------------------------------------------------------------------


        //Shaders------------------------------

        std::vector<char> vertShaderCode = loadShaderFile(vertShaderName);
        std::vector<char> fragShaderCode = loadShaderFile(fragShaderName);

        VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
        VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

        VkPipelineShaderStageCreateInfo vertShaderStageInfo = createShaderStage(VK_SHADER_STAGE_VERTEX_BIT, vertShaderModule, "main");
        VkPipelineShaderStageCreateInfo fragShaderStageInfo = createShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT, fragShaderModule, "main");

        VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };
        //--------------------------------------------------------------

        //vertex input
        VkVertexInputBindingDescription vertexBinding = Vertex::getBindingDescription();
        std::array<VkVertexInputAttributeDescription, 4> vertexAttribute = Vertex::getAttributeDescriptions();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = createVertexInputInfo(vertexBinding, vertexAttribute);
        //---------------

        //input assembly
        VkPipelineInputAssemblyStateCreateInfo inputAssembly = createInputAssemblyInfo(
            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            VK_FALSE,
            nullptr
        );
        //--------------

        //viewport & scissor
        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        auto extent = *VK_WRAPPER->getVkExtent();
        viewport.width = (float)extent.width;
        viewport.height = (float)extent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = {};
        scissor.offset = { 0, 0 };
        scissor.extent = extent;

        VkPipelineViewportStateCreateInfo viewportInfo = createViewportInfo(viewport, scissor);
        //--------------

        //rasterizer
        VkPipelineRasterizationStateCreateInfo rasterizer = createRasterizerInfo(
            VK_FALSE,
            VK_POLYGON_MODE_FILL,
            VK_CULL_MODE_BACK_BIT,
            VK_FRONT_FACE_COUNTER_CLOCKWISE
        );
        //---------------

        //multisample
        VkPipelineMultisampleStateCreateInfo multisample = createMultisampleInfo();
        //---------------

        //color blend
        VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo colorBlend = createColorBlendInfo(colorBlendAttachment);
        //--------------

        //depth stencil
        VkPipelineDepthStencilStateCreateInfo depthStencil = createDepthStencilInfo();
        //--------------

        auto shader = ShaderBuilder()
            .addDescriptorSetLayoutInfo(descriptorSetLayoutInfo)
            .addUniformBufferObjectInfo(modelInfo)
            .addUniformBufferObjectInfo(cameraInfo)
            .addUniformBufferObjectInfo(lightInfo)
            .addPushConstantRange(materialPushConstant)
            .setLayoutBindings(layoutBindings)
            .setShaderStages(shaderStages, 2)
            .setVertexInputState(vertexInputInfo)
            .setInputAssemblyState(inputAssembly)
            .setViewportState(viewportInfo)
            .setRasterizationState(rasterizer)
            .setMultisampleState(multisample)
            .setColorBlendState(colorBlend)
            .setDepthStencilState(depthStencil)
            .createShader();

        vkDestroyShaderModule(VK_WRAPPER->getDevice(), vertShaderModule, nullptr);
        vkDestroyShaderModule(VK_WRAPPER->getDevice(), fragShaderModule, nullptr);

        return shader;

    }


	Shader* ShaderFactory::surfaceShader(const std::string& vertShaderName, const std::string& fragShaderName) {

		//Layout bindings
		std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
        //model
		layoutBindings.push_back(createDescriptorSetLayoutBinding(
			0,									//binding
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,	//binding type
			1,									//descriptor count
			VK_SHADER_STAGE_VERTEX_BIT,			//shader stage
			nullptr								//immutabble samplers
		));

        //camera
		layoutBindings.push_back(createDescriptorSetLayoutBinding(
			1,									//binding
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,	//binding type
			1,									//descriptor count
			VK_SHADER_STAGE_VERTEX_BIT,			//shader stage
			nullptr								//immutabble samplers
		));

        //light
		layoutBindings.push_back(createDescriptorSetLayoutBinding(
			2,									//binding
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,	//binding type
			1,									//descriptor count
			VK_SHADER_STAGE_FRAGMENT_BIT,		//shader stage
			nullptr								//immutabble samplers
		));

        layoutBindings.push_back(createDescriptorSetLayoutBinding(
            3,                                          //binding
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,  //binding type
            1,                                          //descriptor count
            VK_SHADER_STAGE_FRAGMENT_BIT,               //shader stage
            nullptr                                     //immutable samplers
        ));

        layoutBindings.push_back(createDescriptorSetLayoutBinding(
            4,                                          //binding
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,  //binding type
            1,                                          //descriptor count
            VK_SHADER_STAGE_FRAGMENT_BIT,               //shader stage
            nullptr                                     //immutable samplers
        ));

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo = {};
		descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
		descriptorSetLayoutInfo.pBindings = layoutBindings.data();
		//----------------------------------------------------------------------------

        //UniformBufferObjectInfo-------------
        UniformBufferObjectInfo modelInfo = createUboInfo(Ubo::uTransform::size());
        UniformBufferObjectInfo cameraInfo = createUboInfo(Ubo::uCamera::size());
        UniformBufferObjectInfo lightInfo = createUboInfo(Ubo::uLight::size());
        //UniformBufferObjectInfo materialInfo = createUboInfo(sizeof(Ubo::uMaterialProperties) - shaderItemSize);

        //pushConstant
        VkPushConstantRange materialPushConstant = createPushConstantRange(
            VK_SHADER_STAGE_FRAGMENT_BIT,
            sizeof(Ubo::uMaterialProperties),
            0
        );

        //----------------------------------------------------------------------------------



		//Shaders------------------------------

		std::vector<char> vertShaderCode = loadShaderFile(vertShaderName);
        std::vector<char> fragShaderCode = loadShaderFile(fragShaderName);

		VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
		VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

	    VkPipelineShaderStageCreateInfo vertShaderStageInfo = createShaderStage(VK_SHADER_STAGE_VERTEX_BIT, vertShaderModule, "main");
        VkPipelineShaderStageCreateInfo fragShaderStageInfo = createShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT, fragShaderModule, "main");

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };
		//--------------------------------------------------------------

		//vertex input
        VkVertexInputBindingDescription vertexBinding = Vertex::getBindingDescription();
        std::array<VkVertexInputAttributeDescription, 4> vertexAttribute = Vertex::getAttributeDescriptions();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = createVertexInputInfo(vertexBinding, vertexAttribute);
		//---------------
		
		//input assembly
		VkPipelineInputAssemblyStateCreateInfo inputAssembly = createInputAssemblyInfo(
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			VK_FALSE,
			nullptr
		);
		//--------------

		//viewport & scissor
        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        auto extent = *VK_WRAPPER->getVkExtent();
        viewport.width = (float)extent.width;
        viewport.height = (float)extent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = {};
        scissor.offset = { 0, 0 };
        scissor.extent = extent;

		VkPipelineViewportStateCreateInfo viewportInfo = createViewportInfo(viewport, scissor);
		//--------------
        
		//rasterizer
		VkPipelineRasterizationStateCreateInfo rasterizer = createRasterizerInfo(
			VK_FALSE,
			VK_POLYGON_MODE_FILL,
			VK_CULL_MODE_BACK_BIT,
			VK_FRONT_FACE_COUNTER_CLOCKWISE
		);
		//---------------

		//multisample
		VkPipelineMultisampleStateCreateInfo multisample = createMultisampleInfo();
		//---------------

		//color blend
        VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlend = createColorBlendInfo(colorBlendAttachment);
		//--------------

		//depth stencil
		VkPipelineDepthStencilStateCreateInfo depthStencil = createDepthStencilInfo();
		//--------------

		auto shader = ShaderBuilder()
            .addDescriptorSetLayoutInfo(descriptorSetLayoutInfo)
            .addUniformBufferObjectInfo(modelInfo)
            .addUniformBufferObjectInfo(cameraInfo)
            .addUniformBufferObjectInfo(lightInfo)
            .addPushConstantRange(materialPushConstant)
            .setLayoutBindings(layoutBindings)
			.setShaderStages(shaderStages, 2)
			.setVertexInputState(vertexInputInfo)
			.setInputAssemblyState(inputAssembly)
			.setViewportState(viewportInfo)
			.setRasterizationState(rasterizer)
			.setMultisampleState(multisample)
			.setColorBlendState(colorBlend)
			.setDepthStencilState(depthStencil)
			.createShader();

		vkDestroyShaderModule(VK_WRAPPER->getDevice(), vertShaderModule, nullptr);
		vkDestroyShaderModule(VK_WRAPPER->getDevice(), fragShaderModule, nullptr);

		return shader;
	}

	std::vector<char> ShaderFactory::loadShaderFile(const std::string& filename) {
		//abre o arquivo, come�a a ler pelo final e em binario
		std::ifstream file(SHADER_FOLDER + filename, std::ios::ate | std::ios::binary);

		//se n abriu o arquivo, pula fora
		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}

		//Cria o buffer com o tamanho do arquivo
		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		//volta para o come�o do arquivo
		file.seekg(0);
		//passa os dados do arquio pro buffer
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}
	VkDescriptorSetLayoutBinding ShaderFactory::createDescriptorSetLayoutBinding(
		uint32_t binding,
		VkDescriptorType type,
		uint32_t descriptorCount,
		VkShaderStageFlags flags,
		const VkSampler * pImmutableSamplers
	) {

		VkDescriptorSetLayoutBinding descriptorBinding = {};
		descriptorBinding.binding = binding;
		descriptorBinding.descriptorType = type;
		descriptorBinding.descriptorCount = descriptorCount;
		descriptorBinding.stageFlags = flags;
		descriptorBinding.pImmutableSamplers = pImmutableSamplers;

		return descriptorBinding;
	}

    VkPushConstantRange ShaderFactory::createPushConstantRange(VkShaderStageFlags stage, uint32_t range, uint32_t offset) {
        VkPushConstantRange pushConstant = {};
        pushConstant.stageFlags = stage;
        pushConstant.size = range;
        pushConstant.offset = offset;
        return pushConstant;
    }

    UniformBufferObjectInfo ShaderFactory::createUboInfo(VkDeviceSize range) {
        
        VkDeviceSize minAlignment = VK_WRAPPER->getPhysicalDeviceProperties().limits.minUniformBufferOffsetAlignment;
        UniformBufferObjectInfo uboInfo = {};
        uboInfo.offset = (range / minAlignment) * minAlignment + ((range % minAlignment) > 0 ? minAlignment : 0);
        uboInfo.range = range;
        return uboInfo;
    }

	VkShaderModule ShaderFactory::createShaderModule(const std::vector<char>& code) {

		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		//Cast de char* para uint32_t* 
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(VK_WRAPPER->getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shader module!");
		}
		return shaderModule;
	}

	VkPipelineShaderStageCreateInfo ShaderFactory::createShaderStage(
		VkShaderStageFlagBits stage,
		VkShaderModule module,
		const char * pName
	) {
		
		VkPipelineShaderStageCreateInfo shaderStageInfo = {};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = stage;
		shaderStageInfo.module = module;
		shaderStageInfo.pName = pName;
		return shaderStageInfo;

	}

	VkPipelineVertexInputStateCreateInfo ShaderFactory::createVertexInputInfo(
        VkVertexInputBindingDescription& bindingDescription,
        std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions
    ) {

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};

		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;

		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		return vertexInputInfo;
	}

	VkPipelineInputAssemblyStateCreateInfo ShaderFactory::createInputAssemblyInfo(
		VkPrimitiveTopology topology,
		VkBool32 primitiveRestartEnable,
		const void * pNext
	) {
		
		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = topology;
		inputAssembly.primitiveRestartEnable = primitiveRestartEnable;
		inputAssembly.pNext = pNext;
		return inputAssembly;
	}
	VkPipelineViewportStateCreateInfo ShaderFactory::createViewportInfo(VkViewport& viewport, VkRect2D& scissor) {
		

		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		return viewportState;

	}

	VkPipelineRasterizationStateCreateInfo ShaderFactory::createRasterizerInfo(
		const VkBool32& depthClamp,
		VkPolygonMode fillMode,
		VkCullModeFlags cullMode,
		VkFrontFace frontFace
	) {
		
		VkPipelineRasterizationStateCreateInfo rasterizer = {};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = depthClamp;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = fillMode;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = cullMode;
		rasterizer.frontFace = frontFace;

		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f;
		rasterizer.depthBiasClamp = 0.0f;
		rasterizer.depthBiasSlopeFactor = 0.0f;

		return rasterizer;
	}

	VkPipelineMultisampleStateCreateInfo ShaderFactory::createMultisampleInfo() {
		
		VkPipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional

		return multisampling;
	}

	VkPipelineColorBlendStateCreateInfo ShaderFactory::createColorBlendInfo(VkPipelineColorBlendAttachmentState& colorBlendAttachment) {

		VkPipelineColorBlendStateCreateInfo colorBlending = {};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f; // Optional
		colorBlending.blendConstants[1] = 0.0f; // Optional
		colorBlending.blendConstants[2] = 0.0f; // Optional
		colorBlending.blendConstants[3] = 0.0f; // Optional

		return colorBlending;
	}

	VkPipelineDepthStencilStateCreateInfo ShaderFactory::createDepthStencilInfo(){
		
		VkPipelineDepthStencilStateCreateInfo depthStencil = {};

		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = VK_TRUE;
		depthStencil.depthWriteEnable = VK_TRUE;
		depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;

		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.minDepthBounds = 0.0f; // Optional
		depthStencil.maxDepthBounds = 1.0f; // Optional

		depthStencil.stencilTestEnable = VK_FALSE;
		depthStencil.front = {}; // Optional
		depthStencil.back = {}; // Optional

		return depthStencil;

	}
}


