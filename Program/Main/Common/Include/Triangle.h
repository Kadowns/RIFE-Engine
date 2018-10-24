#pragma once

#include <Application.h>
#include <Scene.h>
#include <RifeCore.h>
#include <RifeTools.h>
#include <RifeGraphics.h>

#include <RotatingCube.h>

using namespace Rife::Graphics;
using namespace Rife::Tools;
using namespace Rife::Base;
using namespace Rife::Math;

const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f, 0.5f}, {0.3f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
	{{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
	{{0.5f, 0.5f, 0.5f}, {0.3f, 0.5f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f, 0.5f, 0.5f}, {0.3f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},

	{{-0.5f, -0.5f, -0.5f}, {0.3f, 0.5f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
	{{-0.5f, -0.5f, 0.5f}, {0.3f, 0.5f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
	{{-0.5f, 0.5f, 0.5f}, {0.3f, 1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
	{{-0.5f, 0.5f, -0.5f}, {0.3f, 1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},

	{{0.5f, -0.5f, 0.5f}, {0.3f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, -0.5f, -0.5f}, {0.3f, 0.5f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, 0.5f, -0.5f}, {0.3f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, 0.5f, 0.5f}, {0.3f, 0.5f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},

	{{0.5f, -0.5f, -0.5f}, {0.3f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
	{{-0.5f, -0.5f, -0.5f}, {0.3f, 0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
	{{-0.5f, 0.5f, -0.5f}, {0.3f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
	{{0.5f, 0.5f, -0.5f}, {0.3f, 0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},

	{{-0.5f, -0.5f, 0.5f}, {0.3f, 1.0f, 1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
	{{-0.5f, -0.5f, -0.5f}, {0.3f, 0.5f, 1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
	{{0.5f, -0.5f, -0.5f}, {0.3f, 1.0f, 1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
	{{0.5f, -0.5f, 0.5f}, {0.3f, 0.5f, 1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},

	{{-0.5f, 0.5f, 0.5f}, {0.3f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, 0.5f, 0.5f}, {0.3f, 0.5f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, 0.5f, -0.5f}, {0.3f, 0.5f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
	{{-0.5f, 0.5f, -0.5f}, {0.3f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}}
};


const std::vector<uint32_t> indices = {
0, 1, 2, 2, 3, 0,
4, 5, 6, 6, 7, 4,
8, 9, 10, 10, 11, 8,
12, 13, 14, 14, 15, 12,
16, 17, 18, 18, 19, 16,
20, 21, 22, 22, 23, 20
};

class Triangle : public Scene {
private:

	const uint32_t numberOfCubes = 5;

    vk::Wrapper* vkWrapper;
	void updateUniformBuffer(uint32_t currentImage);
    std::vector<GameObject*> gameObjects;

    Keyboard *keys;
	Camera* m_camera;

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

