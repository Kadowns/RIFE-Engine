#pragma once
#include <GraphicsDependencys.h>
#include <UniformBufferObject.h>
#include <Object.h>
#include <Camera.h>

class Renderer {
public:
	virtual void draw() = 0;
	virtual void createCommandBuffers() = 0;
	virtual void updateTransformInformation(glm::mat4& vp, uint32_t imageIndex, float time) = 0;
protected:
	Entity::Object* p_father;
	
	Graphics::UniformBufferObject m_ubo;
	std::vector<VkCommandBuffer> m_commandBuffers;

};