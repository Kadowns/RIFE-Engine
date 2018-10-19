#pragma once
#ifndef INCLUDE_COMPONENT
#define INCLUDE_COMPONENT
#include <Object.h>

namespace Rife::Base {
	class Component : Object {

	public:

        virtual void setup() { m_setup = true; }
        virtual void awake() { m_awake = true; }
        virtual void update();

	protected:

        Object* p_parentObject;

    private:

        void setParentObject(Object*);

        bool m_setup = false, m_awake = false;

	};
}
#endif //INCLUDE_COMPONENT
