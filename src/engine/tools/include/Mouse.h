#pragma once

#include <RifeMath.h>
#include <Event.h>

#include <set>

#define MOUSE Rife::Tools::Mouse::getInstance()

typedef Rife::Base::Event<double, double> OnMouseMove;

struct GLFWwindow;

namespace Rife::Tools {

    class Mouse {

    public:

        static Mouse* getInstance();

        void setWindow(GLFWwindow* window);

        void mouseMove(double x, double y);

        bool isPressed(int key);
        bool isDown(int key);
        bool isReleased(int key);

        void setCursor(bool visible);

        void set(int button, int action);

        void update();

        OnMouseMove& onMouseMove() { return m_onMouseMoveEvent; }

    private:

        Mouse();

        OnMouseMove m_onMouseMoveEvent;

        GLFWwindow* p_window;

        glm::dvec2 m_lastPosition;

        std::set<int> m_pressedKeys, m_downKeys, m_releasedKeys;

        static Mouse* s_instance;

        bool m_firstMove = true;

    };
}