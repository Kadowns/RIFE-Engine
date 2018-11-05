#pragma once

#include <Application.h>

using namespace Rife::Graphics;
using namespace Rife::Tools;
using namespace Rife::Base;
using namespace Rife::Math;

class ScenePlayer : public Scene {
private:

    std::vector<GameObject*> gameObjects;

public:
    ScenePlayer();
    ~ScenePlayer();

    void init();
    void awake();
    void update();
    void draw();
    void deinit();

    void windowResized(const uint32_t& width, const uint32_t& height);
};

