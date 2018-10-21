#pragma once
#include <Scene.h>

class Test : public Scene {
public:
    void init();
    void awake();
    void update();
    void draw();
    void deinit();
};