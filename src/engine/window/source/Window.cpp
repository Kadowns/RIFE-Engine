#include <Window.h>

#include <thread>

//--------------------------Singleton
Window *Window::s_instance = nullptr;

Window *Window::getInstance() {
    return s_instance != nullptr ? s_instance : (s_instance = new Window());
}
//----------------------------------
void Window::windowResizedCallback(GLFWwindow* window, int width, int height) {
	auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	app->m_width = width;
	app->m_height = height;
	app->m_windowResized = true;
}

void Window::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	KEYBOARD->set(key, action);
}

void Window::mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
    MOUSE->mouseMove(xpos, ypos);
}

void Window::mouseButtonCallback(GLFWwindow * window, int button, int action, int mods) {
    MOUSE->set(button, action);
}


int Window::getWidth() {
    return m_width;
}

int Window::getHeight() {
    return m_height;
}

void Window::define(Rife::Base::Scene *scene, const std::string &title, int width, int height) {
    m_scene = scene;
    m_title = title;
    m_width = width;
    m_height = height;
    m_initialized = false;
}

//---------------------------Constructors
Window::Window(Rife::Base::Scene *scene, const std::string &title, int width, int height) {
    m_scene = scene;
    m_title = title;
    m_width = width;
    m_height = height;
    s_instance = this;
}

Window::Window(Rife::Base::Scene *scene, const std::string &title) : Window(scene, title, 800, 600) {}

Window::Window(Rife::Base::Scene *scene) : Window(scene, "Game") {}

Window::Window() {};
//---------------------------

//Inicializa a porra toda
void Window::init() {

    initGlfw();
    initVulkan();
	DATABASE::loadData();
    m_scene->loadScene();
    m_initialized = true;
}

//Inicializa a glfw
void Window::initGlfw() {

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
    glfwShowWindow(m_window);

    //inputs
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(m_window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    //callbacks
    glfwSetFramebufferSizeCallback(m_window, windowResizedCallback);
	glfwSetKeyCallback(m_window, keyboardCallback);
    glfwSetCursorPosCallback(m_window, mouseMoveCallback);
    glfwSetMouseButtonCallback(m_window, mouseButtonCallback);

    MOUSE->setWindow(m_window);
   
}

//Inicializa a vulkan
void Window::initVulkan() {
    m_vulkanBase = new Rife::Graphics::VulkanBase(m_window);
}

//loop principal, n tem segredo n�
void Window::loop() {      
    do {
        TIME->earlyUpdate();

        m_scene->onUpdate();
        m_scene->onLateUpdate();
        m_scene->onRender();

        
        KEYBOARD->update();
        MOUSE->update();
        glfwPollEvents();

        TIME->lateUpdate();

    } // se o maluco apertar os bot�o de sair, vc sai
    while (!KEYBOARD->isPressed(GLFW_KEY_ESCAPE) && glfwWindowShouldClose(m_window) == GLFW_FALSE);
   
}

void Window::terminate() {

    vkDeviceWaitIdle(Rife::Graphics::Vulkan::device);
    m_scene->onTerminate();
	DATABASE::unloadData();
    m_vulkanBase->terminateVulkan();

    delete m_vulkanBase;
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

//Fun��o chamada pra inicializar a scene e manter o loop
void Window::show() {

    if (!m_initialized)
        init();

    loop();
    terminate();
}
