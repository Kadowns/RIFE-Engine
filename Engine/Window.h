#pragma once

//gl dependencies
#ifndef GLDEP
#define GLDEP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif // !GLDEP

#include "Scene.h"
#include "Keyboard.h"
#include "Time.h"

#include <chrono>
#include <thread>
#include <string>


class Window {
private :
	bool m_initialized;
	Scene *m_scene;
	int m_width;
	int m_height;
	std::string m_title;
	GLFWwindow *m_window;
	GLFWerrorfun m_errorCallback;
	
	void init();
	void loop();
		
public:

	Window(Scene *scene, const std::string &title, int width, int height);
	Window(Scene *scene, const std::string &title);
	Window(Scene *scene);
	virtual ~Window() = default;

    void show();	
};