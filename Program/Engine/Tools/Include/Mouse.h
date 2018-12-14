#pragma once

#include <RifeMath.h>
#include <Event.h>

#define MOUSE Rife::Tools::Mouse::getInstance()

typedef Rife::Base::Event<double, double> MouseMoveEvent;

namespace Rife::Tools {

    class Mouse {

    public:

        static Mouse* getInstance();

        void mouseMove(double x, double y);

        MouseMoveEvent& OnMouseMove() { return m_onMouseMoveEvent; }

    private:

        Mouse();

        MouseMoveEvent m_onMouseMoveEvent;

        glm::dvec2 m_lastPosition;

        static Mouse* s_instance;

        bool m_firstMove = true;

    };
}