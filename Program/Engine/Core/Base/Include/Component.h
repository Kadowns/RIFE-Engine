#pragma once
#include <Object.h>

class GameObject;

namespace Rife::Base {

	class Component : Object {

	public:

        virtual void setup() { m_setup = true; }
        virtual void awake() { m_awake = true; }
        virtual void update();

	protected:

        GameObject* p_gameObject;

    private:

        void setParentObject(GameObject*);

        bool m_setup = false, m_awake = false;

	};
}
