#include "Include/Application.h"

//--------------------------Singleton
Application* Application::m_instance = nullptr;

Application* Application::getInstance() {
	return m_instance != nullptr ? m_instance : (m_instance = new Application());
}
//----------------------------------

int Application::getWidth() {
	return m_width;
}

int Application::getHeight() {
	return m_height;
}

void Application::define(Scene * scene, const std::string & title, int width, int height) {
	m_scene = scene;
	m_title = title;
	m_width = width;
	m_height = height;
	m_initialized = false;
}

//---------------------------Constructors
Application::Application(Scene *scene, const std::string &title, int width, int height){
	m_scene = scene;
	m_title = title;
	m_width = width;
	m_height = height;
	m_instance = this;
}

Application::Application(Scene *scene, const std::string &title) : Application(scene, title, 800, 600) {}

Application::Application(Scene *scene): Application(scene, "Game") {}

Application::Application() {};
//---------------------------

//Inicializa a porra toda
void Application::init(){

	initGlfw();
	initVulkan();
	m_initialized = true;
}

//Inicializa a glfw
void Application::initGlfw() {

	if (!glfwInit())
		throw std::exception("Unable to initialize glfw");


	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);

	if (m_window == NULL)
		throw std::exception("Failed to create the GLFW Window");

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	glfwSwapInterval(1);
	glfwShowWindow(m_window);
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE);
	
}

//Inicializa a vulkan
void Application::initVulkan() {
	m_vkWrapper = new vk::Wrapper();
	m_vkWrapper->initializeVulkan(m_window, m_width, m_height);
}

//loop principal, n tem segredo né
void Application::loop(){
	m_scene->init();
	do {
		TIME->earlyUpdate();
		
		double time = TIME->getLastFrameTime();
		m_scene->update(time);
		m_scene->draw();
		printf("Time: %f, FPS : %f, Total frames: %i\n", time, TIME->getFPS(), TIME->getTotalFrames());
		
		KEYBOARD->update();
		glfwPollEvents();

		TIME->lateUpdate();

	} // se o maluco apertar os botão de sair, vc sai
	while (glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(m_window) == GLFW_FALSE);
	m_scene->deinit();
}

void Application::terminate() {

	m_vkWrapper->terminateVulkan();
	delete m_vkWrapper;
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

//Função chamada pra inicializar a scene e manter o loop
void Application::show() {

	if (!m_initialized)
		init();

	loop();
	terminate();
}
