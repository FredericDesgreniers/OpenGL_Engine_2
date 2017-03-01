#pragma once

#include "iostream"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "ScreenManager.h"
#include "ScreenCreateCurve.h"

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 800


class Engine{
public:
    
    static Engine* getInstance();

	/**
	 * Callback on opengl error
	 */
	void error_callback(int error, const char* description);

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	void window_size_callback(GLFWwindow* window, int width, int height);

	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	/**
	* Run Engine
	*/
	void run();

	/**
	* Window width
	*/
	int width;
	/**
	* Window height
	*/
	int height;

	GLFWwindow* window;
private:
    Engine();
	static Engine *instance;

	ScreenManager screenManager;


	


	/**
	 * Initialize Engine
	 */
	void initialize();

	/**
	 * Initialize GLFW
	 */
	void initializeGLFW();

	/**
	 * Initialize GLEW
	 */
	void initializeGLEW();

	

	int lastMouseX, lastMouseY;
	
};
