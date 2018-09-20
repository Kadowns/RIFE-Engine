#pragma once

#include "VkUtilities.h"
#include "Clock.h"
#include "Scene.h"

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>

#include <vector>
#include <map>
#include <chrono>
#include <thread>
#include <GLFW/glfw3.h>

#define APPLICATION Application::getInstance()

//----------------------------------------------------
class Application {
private :

    static Application *m_instance;

    int m_width;
    int m_height;
    std::string m_title;
    bool m_initialized = false;

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

    static Application *getInstance();

    int getWidth();

    int getHeight();

    void define(Scene *scene, const std::string &title, int width, int height);

    Application(Scene *scene, const std::string &title, int width, int height);

    Application(Scene *scene, const std::string &title);

    Application(Scene *scene);

    virtual ~Application() = default;

    void show();
};