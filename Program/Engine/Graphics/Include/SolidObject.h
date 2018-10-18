#pragma once
#include <Object.h>
#include <MeshRenderer.h>


namespace Entity {

	class SolidObject : public Object {
	public:
		SolidObject(Mesh* mesh);
		~SolidObject();
		void setup();
		void update(float secs);

		Mesh* getMesh() { return m_mesh; }
		MeshRenderer* getMeshRenderer() { return m_renderer; }

	private:
		Mesh* m_mesh;
		MeshRenderer* m_renderer;

	};
}