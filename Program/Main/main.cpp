#include <Application.h>
#include <ScenePlayer.h>

#include <thread>

int main() {

    srand(time(0));

    int threads = std::thread::hardware_concurrency();
    std::cout << "Available threads: " << threads << "\n";

    Scene *scene = new ScenePlayer();
    Application *app = new Application(scene, "Rife&Vulkan", 1080, 720);
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