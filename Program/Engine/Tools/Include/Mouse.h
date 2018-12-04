#pragma once

#include <RifeMath.h>
#include <Event.h>
#include <functional>

#define MOUSE Rife::Tools::Mouse::getInstance()

typedef Rife::Base::Event<double, double> MouseMoveEvent;

namespace Rife::Tools {


    class Mouse {
    public:

        static Mouse* getInstance();

        void updatePosition(double x, double y);

        void setUpdatePositionCallback(void*, std::function<void(void* caller, double xoffset, double yoffset)> callback);

        MouseMoveEvent& OnMouseMove() { return m_onMouseMove; }

    private:

        Mouse();

        MouseMoveEvent m_onMouseMove;

        std::vector<std::pair<void*, std::function<void(void* caller, double xoffset, double yoffset)>>> m_callbacks;

        glm::dvec2 m_lastPosition;

        static Mouse* s_instance;

        bool m_firstMove = true;

    };
}