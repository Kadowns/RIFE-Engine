#pragma once
#include <Scene.h>

class Test : public Scene {
public:
	void init();
	void update(float time);
	void draw();
	void deinit();
};