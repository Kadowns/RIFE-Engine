#include <Mouse.h>
#include <GLFW/glfw3.h>

namespace Rife::Tools {
   
    Mouse::Mouse() {}

    Mouse* Mouse::s_instance = nullptr;

    Mouse* Mouse::getInstance() {
        return s_instance != nullptr ? s_instance : (s_instance = new Mouse());
    }

    void Mouse::setWindow(GLFWwindow* window) {
        p_window = window;
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

    bool Mouse::isPressed(int key) {
        return  m_pressedKeys.find(key) != m_pressedKeys.end();
    }

    bool Mouse::isDown(int key) {
        return m_downKeys.find(key) != m_downKeys.end();
    }

    bool Mouse::isReleased(int key) {
        return m_releasedKeys.find(key) != m_releasedKeys.end();
    }

    void Mouse::setCursor(bool visible) {
        if (visible) {
            glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }    
    }

    void Mouse::set(int button, int action) {
        if (action == GLFW_PRESS) {
            m_downKeys.insert(button);
            m_pressedKeys.insert(button);
        }
        else if (action == GLFW_RELEASE) {
            m_downKeys.erase(button);
            m_releasedKeys.insert(button);
        }
    }

    void Mouse::update() {
        m_pressedKeys.clear();
        m_releasedKeys.clear();
    }
}
