#pragma once

#include <RifeCore.h>
#include <RifeTools.h>
#include <RifeGraphics.h>

#include <Database.h>
#include <Scene.h>

#include <vector>

#define APPLICATION Application::getInstance()

//----------------------------------------------------
class Application {
private :

    static Application *s_instance;

    int m_width;
    int m_height;
    std::string m_title;
    bool m_initialized = false;
	bool m_windowResized = false;

    Scene *m_scene;
    GLFWwindow *m_window;
    GLFWerrorfun m_errorCallback;

    Rife::Graphics::VulkanData *m_vulkanData;
    Rife::Graphics::VulkanBase *m_vulkanBase;

    Application();

    void init();

    void initGlfw();

    void initVulkan();

    void loop();

    void terminate();

public:

	static void onWindowResized(GLFWwindow *window, int width, int height);

	static void onKeyboardAction(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void onMouseAction(GLFWwindow* window, double xpos, double ypos);

    static Application *getInstance();

	bool* windowResized() { return &m_windowResized; }
	
    int getWidth();

    int getHeight();

    void define(Scene *scene, const std::string &title, int width, int height);

    Application(Scene *scene, const std::string &title, int width, int height);

    Application(Scene *scene, const std::string &title);

    Application(Scene *scene);

    virtual ~Application() = default;

    void show();
};