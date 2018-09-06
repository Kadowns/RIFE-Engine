#pragma once
#include <GLFW/glfw3.h>
#include <set>

#define KEYBOARD Keyboard::getInstance()

class Keyboard {	
private:
	static Keyboard *m_instance;
	std::set<int> m_pressedKeys, m_downKeys, m_releasedKeys;
	Keyboard();
public:

	~Keyboard();
	static Keyboard* getInstance();
	bool isPressed(int key);
	bool isDown(int key);
	bool isReleased(int key);
	
	void set(int key, int action);

	void update();

};

