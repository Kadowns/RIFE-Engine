#include <Mouse.h>

namespace Rife::Tools {
   
    Mouse::Mouse() {}

    Mouse* Mouse::s_instance = nullptr;

    Mouse* Mouse::getInstance() {
        return s_instance != nullptr ? s_instance : (s_instance = new Mouse());
    }

    void Mouse::mouseMove(double x, double y) {

        if (m_firstMove) {
            m_lastPosition = glm::dvec2(x, y);
            m_firstMove = false;
        }

        float xoffset = x - m_lastPosition.x;
        float yoffset = m_lastPosition.y - y;
        m_lastPosition.x = x;
        m_lastPosition.y = y;

        float sensitivity = 0.05f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;


        m_onMouseMoveEvent(xoffset, yoffset);
    }
}
