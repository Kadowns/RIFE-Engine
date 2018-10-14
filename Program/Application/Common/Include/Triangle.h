#pragma once

#include <Application.h>
#include <Scene.h>
#include <SolidObject.h>

const std::vector<Graphics::Vertex> vertices = {
	{{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
	{{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
	{{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}},
	{{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},

	{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
	{{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
	{{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}},
	{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},

	{{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
	{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
	{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}},
	{{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},

	{{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
	{{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}},
	{{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},

	{{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
	{{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}},
	{{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},

	{{-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
	{{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
	{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}},
	{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}
};

const std::vector<Graphics::Vertex> vertices2 = {
	{{-0.9f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},
	{{0.9f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},
	{{0.9f, 0.5f, 0.5f}, {0.5f, 0.5f, 1.0f, 1.0f}},
	{{-0.9f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}
};

const std::vector<uint32_t> indices2 = {
	0, 1, 2, 2, 3, 0
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
    vk::Wrapper* vkWrapper;
	void updateUniformBuffer(uint32_t currentImage);
	Entity::SolidObject* cube;

public:
    Triangle();
    ~Triangle();

    void init();
    void update(float secs);
    void draw();
    void deinit();
};

