#include <GraphicsDependencys.h>

class Renderer {
public:
	virtual void draw() = 0;
	virtual void recordCmdBuffer() = 0;
protected:
	VkCommandBuffer m_vkCommandBuffer;
	

};