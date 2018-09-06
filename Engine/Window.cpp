#include "Window.h"

//---------------------------Constructors
Window::Window(Scene *scene, const std::string &title, int width, int height){
	m_scene = scene;
	m_title = title;
	m_width = width;
	m_height = height;
	init();
}

Window::Window(Scene *scene, const std::string &title){
	Window(scene, title, 800, 600);
}

Window::Window(Scene *scene){
	Window(scene, "Game");
}
//---------------------------

void Window::init(){

	//glfw
	if (!glfwInit()) 
		throw std::exception("Unable to initialize glfw");
	

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);
	
	if (m_window == NULL) 
		throw std::exception("Failed to create the GLFW Window");
	
	glfwSwapInterval(1);

	//glew
	glfwMakeContextCurrent(m_window);

	if (glewInit() != GLEW_OK)
		throw std::exception("Failed to initialize Glew");

	glfwShowWindow(m_window);
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);

	m_initialized = true;
}

//loop principal, n tem segredo né
void Window::loop(){
	m_scene->init();
	do {
		TIME->earlyUpdate();
		glClear(GL_COLOR_BUFFER_BIT);
		
		double time = TIME->getLastFrameTime();
		m_scene->update(time);
		m_scene->draw();
		printf("Time: %f, FPS : %f, Total frames: %i\n", time, TIME->getFPS(), TIME->getTotalFrames());
		
		KEYBOARD->update();
		glfwSwapBuffers(m_window);
		glfwPollEvents();

		TIME->lateUpdate();

	} // se o maluco apertar os botão de sair, vc sai
	while (glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(m_window) == GL_FALSE);
	m_scene->deinit();
}

//Função chamada pra inicializar a scene e manter o loop
void Window::show() {

	if (!m_initialized)
		init();

	loop();

	glfwDestroyWindow(m_window);
	glfwTerminate();
}
