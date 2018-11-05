#pragma once
#include <Object.h>

namespace Rife::Base {

    class GameObject;

	class Component : Object {

	public:

        virtual void setup();
        virtual void awake();
        virtual void update();

        template<typename T>
        T* getComponent();

        void setParentObject(GameObject*);
	protected:

        GameObject* p_gameObject;

    private:
        bool m_setup = false, m_awake = false;

	};
    template<typename T>
    inline T* Component::getComponent() {
        return p_gameObject->getComponent<T>();
    }
}
