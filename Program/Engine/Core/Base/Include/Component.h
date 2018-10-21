#pragma once
#include <Object.h>

namespace Rife::Base {

    class GameObject;

	class Component : Object {

	public:

        virtual void setup();
        virtual void awake();
        virtual void update();

        void setParentObject(GameObject*);
	protected:

        GameObject* p_gameObject;

    private:
        bool m_setup = false, m_awake = false;

	};
}
