#include <Application.h>
#include <Triangle.h>

int main() {
    Scene *scene = new Triangle();
    Application *app = new Application(scene, "Test", 800, 600);
    try {
        app->show();
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    //s� ta aqui pra dar pra ler os debug callback dps que destruiu o vkinstance e vkcallback
    std::cin.get();

    delete scene;
    delete app;

    return EXIT_SUCCESS;
}