#include "pch.h"
#include "Window.h"
#include "Test.h"


int main() {
	
	Scene *scene = new Test();
	Window *window = new Window(scene, "OpenGL", 912, 768);
	window->show();
	delete scene;
	delete window;
	return 0;
}
