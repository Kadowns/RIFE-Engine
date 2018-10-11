#pragma once

#include <GraphicsIncludes.h>
#include <Clock.h>
#include <Camera.h>
#include "Scene.h"

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>

#include <vector>
#include <map>
#include <chrono>
#include <thread>

#define APPLICATION Application::getInstance()



//----------------------------------------------------
class Application {
private :

    static Application *s_instance;

	gph::Camera* m_camera;

    int m_width;
    int m_height;
    std::string m_title;
    bool m_initialized = false;
	bool m_framebufferResized = false;

    Scene *m_scene;
    GLFWwindow *m_window;
    GLFWerrorfun m_errorCallback;
    vk::Wrapper *m_vkWrapper;

    Application();

    void init();

    void initGlfw();

    void initVulkan();

    void loop();

    void terminate();

public:

	static void frameBufferResizedCallback(GLFWwindow *window, int width, int height);

    static Application *getInstance();

	bool* framebufferResized() { return &m_framebufferResized; }
	
    int getWidth();

    int getHeight();

    vk::Wrapper* getVkWrapper() { return m_vkWrapper; }

    void define(Scene *scene, const std::string &title, int width, int height);

    Application(Scene *scene, const std::string &title, int width, int height);

    Application(Scene *scene, const std::string &title);

    Application(Scene *scene);

    virtual ~Application() = default;

    void show();
};