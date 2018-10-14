#pragma once
#include <GraphicsDependencys.h>
#include <Transform.h>


namespace Entity {

	class Object {
	public:
		virtual void setup() = 0;
		virtual void update(float secs) = 0;

		Transform* getTransform() { return m_transform; }

	protected:
		Transform* m_transform;
	};
}
