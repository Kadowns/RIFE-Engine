#pragma once

#include <RifeCore.h>
#include <RifeTools.h>
#include <RifeGraphics.h>

#include <DataBase.h>
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

    int m_width;
    int m_height;
    std::string m_title;
    bool m_initialized = false;
	bool m_framebufferResized = false;

    Scene *m_scene;
    GLFWwindow *m_window;
    GLFWerrorfun m_errorCallback;
    Rife::Graphics::VulkanBase *m_vkWrapper;

    Application();

    void init();

    void initGlfw();

    void initVulkan();

    void loop();

    void terminate();

public:

	static void frameBufferResizedCallback(GLFWwindow *window, int width, int height);

	static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);

    static Application *getInstance();

	bool* framebufferResized() { return &m_framebufferResized; }
	
    int getWidth();

    int getHeight();

    Rife::Graphics::VulkanBase* getVkWrapper() { return m_vkWrapper; }

    void define(Scene *scene, const std::string &title, int width, int height);

    Application(Scene *scene, const std::string &title, int width, int height);

    Application(Scene *scene, const std::string &title);

    Application(Scene *scene);

    virtual ~Application() = default;

    void show();
};