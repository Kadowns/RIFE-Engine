#pragma once
#include <Object.h>

namespace Rife::Base {

    class GameObject;

	class Component : public Object {

	public:

        template<typename T>
        T* getComponent();

        void setParentObject(GameObject*);

        virtual void onInit() {};
        virtual void onAwake() {};
        virtual void onUpdate() {};
        virtual void onLateUpdate() {};

	protected:

        bool m_enabled = true;

        GameObject* p_gameObject;

    private:
        bool m_setup = false, m_awake = false;

	};
    template<typename T>
    inline T* Component::getComponent() {
        return p_gameObject->getComponent<T>();
    }
}
