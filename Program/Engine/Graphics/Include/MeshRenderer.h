#include <GraphicsDependencys.h>
#include <VkUtilities.h>
#include <Renderer.h>

class MeshRenderer : Renderer {
	void draw();
	void recordCmdBuffer();
public:
	MeshRenderer();
private:
	

};