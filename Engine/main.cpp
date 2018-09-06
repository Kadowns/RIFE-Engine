#include "Test.h"
#include "Window.h"

int main() {

	Scene *scene = new Test();
	Window *window = new Window(scene, "Test");
	delete scene;
	delete window;

    return 0;
}