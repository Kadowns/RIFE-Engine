#include <Keyboard.h>
namespace Rife::Tools {


    Keyboard::Keyboard() {
    }


    Keyboard::~Keyboard() {
    }

    //Singleton-
    Keyboard* Keyboard::s_instance = nullptr;

    Keyboard* Keyboard::getInstance() {
        return s_instance != nullptr ? s_instance : (s_instance = new Keyboard());
    }
    //----------

    bool Keyboard::isPressed(int key) {
        return m_pressedKeys.find(key) != m_pressedKeys.end();
    }

    bool Keyboard::isDown(int key) {
        return m_downKeys.find(key) != m_downKeys.end();
    }

    bool Keyboard::isReleased(int key) {
        return  m_releasedKeys.find(key) != m_releasedKeys.end();
    }

    void Keyboard::set(int key, int action) {
        if (action == GLFW_PRESS)
        {
            m_downKeys.insert(key);
            m_pressedKeys.insert(key);
        }
        else if (action == GLFW_RELEASE)
        {
            m_downKeys.erase(key);
            m_releasedKeys.insert(key);
        }
    }

    void Keyboard::update() {
        m_pressedKeys.clear();
        m_releasedKeys.clear();
    }
}