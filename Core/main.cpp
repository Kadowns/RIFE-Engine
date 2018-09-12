#include <Include/Window.h>
#include <Include/Test.h>

int main() {
	Scene *scene = new Test();
	Window *window = new Window(scene, "Test", 800, 600);
	try {
		window->show();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	//s� ta aqui pra dar pra ler os debug callback dps que destruiu o vkinstance e vkcallback
	system("pause");

	delete scene;
	delete window;

    return EXIT_SUCCESS;
}