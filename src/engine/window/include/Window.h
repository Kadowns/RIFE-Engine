#pragma once

#include <RifeCore.h>
#include <RifeTools.h>
#include <RifeGraphics.h>

#include <Database.h>
#include <Scene.h>

#include <vector>

#define WINDOW Window::getInstance()

//----------------------------------------------------
class Window {
private :

    static Window *s_instance;

    int m_width;
    int m_height;
    std::string m_title;
    bool m_initialized = false;
	bool m_windowResized = false;

    Rife::Base::Scene *m_scene;
    GLFWwindow *m_window;
    GLFWerrorfun m_errorCallback;

    Rife::Graphics::VulkanBase *m_vulkanBase;

    Window();

    void init();

    void initGlfw();

    void initVulkan();

    void loop();

    void terminate();

public:

	static void windowResizedCallback(GLFWwindow *window, int width, int height);

	static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    static Window *getInstance();

	bool* windowResized() { return &m_windowResized; }
	
    int getWidth();

    int getHeight();

    void define(Rife::Base::Scene *scene, const std::string &title, int width, int height);

    Window(Rife::Base::Scene *scene, const std::string &title, int width, int height);

    Window(Rife::Base::Scene *scene, const std::string &title);

    Window(Rife::Base::Scene *scene);

    virtual ~Window() = default;

    void show();
};