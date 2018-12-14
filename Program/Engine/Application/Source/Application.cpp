#include <Application.h>
#include <thread>

//--------------------------Singleton
Application *Application::s_instance = nullptr;

Application *Application::getInstance() {
    return s_instance != nullptr ? s_instance : (s_instance = new Application());
}
//----------------------------------
void Application::glfwWindowResizedCallback(GLFWwindow* window, int width, int height) {
	auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
	app->m_width = width;
	app->m_height = height;
	app->m_windowResized = true;
}

void Application::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	KEYBOARD->set(key, action);
}

void Application::mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
    MOUSE->mouseMove(xpos, ypos);
}


int Application::getWidth() {
    return m_width;
}

int Application::getHeight() {
    return m_height;
}

void Application::define(Scene *scene, const std::string &title, int width, int height) {
    m_scene = scene;
    m_title = title;
    m_width = width;
    m_height = height;
    m_initialized = false;
}

//---------------------------Constructors
Application::Application(Scene *scene, const std::string &title, int width, int height) {
    m_scene = scene;
    m_title = title;
    m_width = width;
    m_height = height;
    s_instance = this;
}

Application::Application(Scene *scene, const std::string &title) : Application(scene, title, 800, 600) {}

Application::Application(Scene *scene) : Application(scene, "Game") {}

Application::Application() {};
//---------------------------

//Inicializa a porra toda
void Application::init() {

    initGlfw();
    initVulkan();
	DATABASE::loadData();
    m_initialized = true;
}

//Inicializa a glfw
void Application::initGlfw() {

    if (!glfwInit())
        throw std::runtime_error("Unable to initialize glfw");


    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);


    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);

    if (m_window == nullptr)
        throw std::runtime_error("Failed to create the GLFW Window");

	glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, glfwWindowResizedCallback);
    glfwShowWindow(m_window);
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetKeyCallback(m_window, keyboardCallback);
    glfwSetCursorPosCallback(m_window, mouseMoveCallback);
   
}

//Inicializa a vulkan
void Application::initVulkan() {
    m_vulkanData = new Rife::Graphics::VulkanData();
    m_vulkanBase = new Rife::Graphics::VulkanBase(m_window);
    m_vulkanBase->initialSetup();
}

//loop principal, n tem segredo n�
void Application::loop() {
    m_scene->init();
	m_vulkanBase->finalSetup();
    m_scene->awake();
    do {
        TIME->earlyUpdate();

        m_scene->update();
        m_scene->draw();

        KEYBOARD->update();
        glfwPollEvents();

        TIME->lateUpdate();

    } // se o maluco apertar os bot�o de sair, vc sai
    while (glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(m_window) == GLFW_FALSE);
    vkDeviceWaitIdle(VK_DATA->getDevice());
   
}

void Application::terminate() {

    m_scene->deinit();
	DATABASE::unloadData();
    m_vulkanBase->terminateVulkan();
    delete m_vulkanBase;
    delete m_vulkanData;
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

//Fun��o chamada pra inicializar a scene e manter o loop
void Application::show() {

    if (!m_initialized)
        init();

    loop();
    terminate();
}
