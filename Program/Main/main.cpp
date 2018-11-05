#include <Application.h>
#include <ScenePlayer.h>

int main() {

    srand(time(0));

    Scene *scene = new ScenePlayer();
    Application *app = new Application(scene, "Test", 800, 600);
    try {
        app->show();
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
		std::cin.get();
        return EXIT_FAILURE;
    }

  
    delete scene;
    delete app;

	//s� ta aqui pra dar pra ler os debug callback dps que destruiu o vkinstance e vkcallback
	std::cin.get();

    return EXIT_SUCCESS;
}