#pragma once
#include <Object.h>

namespace Rife::Base {

	class Component : Object {

	public:

        void setParentObject(void*);
	protected:

        void* p_gameObject;

    private:

	};
}
