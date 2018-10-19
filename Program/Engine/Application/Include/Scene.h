#pragma once

#include <Keyboard.h>

class Scene {
protected:
    Keyboard *keys = KEYBOARD;
public:
    virtual void init() = 0;
    virtual void update(float secs) = 0;
    virtual void draw() = 0;
    virtual void deinit() = 0;
};
