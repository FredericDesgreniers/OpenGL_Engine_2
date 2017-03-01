#pragma once

#include "ScreenManager.h"
#include "GL/glew.h"
#include <GLFW/glfw3.h>

class ScreenManager;

class Screen
{
protected:
	ScreenManager* screenManager;

	

public:
	Screen(ScreenManager* screenManager);

	virtual void tick();
	virtual void render();

	virtual void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

	virtual void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	virtual void window_size_callback(GLFWwindow* window, int width, int height);
	
	virtual void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};
