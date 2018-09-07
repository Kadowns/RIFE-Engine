#include "Window.h"
#include "Test.h"

int main() {

	Scene *scene = new Test();
	Window *window = new Window(scene, "Test");
	window->show();
	delete scene;
	delete window;

    return 0;
}