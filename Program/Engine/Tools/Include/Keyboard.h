#pragma once

#include <set>
#include <GraphicsLibs.h>


#define KEYBOARD Rife::Tools::Keyboard::getInstance()

namespace Rife::Tools {

    class Keyboard {
    public:

        ~Keyboard();
        static Keyboard* getInstance();
        bool isPressed(int key);
        bool isDown(int key);
        bool isReleased(int key);

        void set(int key, int action);

        void update();

    private:
        Keyboard();
        static Keyboard *s_instance;
        std::set<int> m_pressedKeys, m_downKeys, m_releasedKeys;
    };
}


