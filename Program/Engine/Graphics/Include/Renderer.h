#pragma once
#include <GraphicsDependencys.h>
#include <Object.h>
#include <Camera.h>

class Renderer {
public:
	virtual void draw() = 0;
	virtual void recordCmdBuffer() = 0;
	virtual void setMVP(glm::mat4& vp) = 0;
protected:
	Entity::Object* p_father;
	Entity::FinalTransform m_finalTransform;
	
	std::vector<VkCommandBuffer> m_commandBuffers;

};