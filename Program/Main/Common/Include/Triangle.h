#pragma once

#include <Application.h>

using namespace Rife::Graphics;
using namespace Rife::Tools;
using namespace Rife::Base;
using namespace Rife::Math;

class Triangle : public Scene {
private:

    std::vector<GameObject*> gameObjects;

	Camera* m_camera;

    Material* mat;

public:
    Triangle();
    ~Triangle();

    void init();
    void awake();
    void update();
    void draw();
    void deinit();

    void windowResized(const uint32_t& width, const uint32_t& height);
};

