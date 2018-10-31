#pragma once

#include <Application.h>
#include <RifeCore.h>
#include <RifeTools.h>
#include <RifeGraphics.h>

#include <RotatingCube.h>
#include <Movable.h>

using namespace Rife::Graphics;
using namespace Rife::Tools;
using namespace Rife::Base;
using namespace Rife::Math;

class Triangle : public Scene {
private:

	const uint32_t numberOfCubes = 5;

    VulkanBase* vkWrapper;
	void updateUniformBuffer(uint32_t currentImage);
    std::vector<GameObject*> gameObjects;

    Keyboard *keys;
	Camera* m_camera;

    Material* mat;
    Texture* diffTex;
    Texture* specTex;
    Mesh* cube;
    Mesh* plane;

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

