#pragma once

#include <Application.h>
#include <Scene.h>


class Triangle : public Scene {
private:

    vk::Wrapper* vkWrapper;
	void updateUniformBuffer(uint32_t currentImage);

public:
    Triangle();
    ~Triangle();

    void init();
    void update(float secs);
    void draw();
    void deinit();
};

