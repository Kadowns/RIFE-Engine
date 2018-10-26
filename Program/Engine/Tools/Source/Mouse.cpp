#include <Mouse.h>

namespace Rife::Tools {
   
    Mouse::Mouse() : m_lastPosition(glm::dvec2(400.0, 300.0)) {}

    Mouse* Mouse::s_instance = nullptr;

    Mouse* Mouse::getInstance() {
        return s_instance != nullptr ? s_instance : (s_instance = new Mouse());
    }

    void Mouse::updatePosition(double x, double y) {
        float xoffset = x - m_lastPosition.x;
        float yoffset = m_lastPosition.y - y;
        m_lastPosition.x = x;
        m_lastPosition.y = y;

        float sensitivity = 0.05f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        for (int i = 0; i < m_callbacks.size(); i++) {
            m_callbacks[i].second(m_callbacks[i].first, xoffset, yoffset);
        }
    }

    void Mouse::setUpdatePositionCallback(void* caller, std::function<void(void* caller, double xoffset, double yoffset)> callback) {
        m_callbacks.push_back(std::pair<void*, std::function<void(void* caller, double xoffset, double yoffset)>>(caller, callback));
    }
}
