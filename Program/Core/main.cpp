#include <Application.h>
#include <Test.h>

int main() {
	Scene *scene = new Test();
	Application *app = new Application(scene, "Test", 800, 600);
	try {
		app->show();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	//s� ta aqui pra dar pra ler os debug callback dps que destruiu o vkinstance e vkcallback
	system("pause");

	delete scene;
	delete app;

    return EXIT_SUCCESS;
}