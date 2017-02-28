#include "Engine.h"


Engine* Engine::instance;

void global_error_callback(int error, const char* description)
{
	Engine::getInstance()->error_callback(error, description);
}
void global_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	Engine::getInstance()->key_callback(window, key, scancode, action, mode);
}
void global_mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	
}
void global_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Engine::getInstance()->mouse_button_callback(window, button, action, mods);
}
void global_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	
}
void global_window_size_callback(GLFWwindow* window, int width, int height)
{
	
}
void global_framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	
}

Engine* Engine::getInstance()
{
    if(instance == nullptr)
    {
        instance = new Engine();
    }

	return instance;
}

Engine::Engine():screenManager(DEFAULT_WIDTH, DEFAULT_HEIGHT){
    std::cout << "Starting Engine" << std::endl;
	initialize();
}

void Engine::run()
{
	std::cout << "----------RUNNING----------"<< std::endl;

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	
		Screen* screen = screenManager.getCurrentScreen();
		
		if(screen != nullptr)
		{
			screen->tick();

			screen->render();
		}
		glfwSwapBuffers(window);

	}

	glfwTerminate();
}

void Engine::initialize()
{
	std::cout << "----------INITIALIZING---------" << std::endl;

	//Set window dimensions
	width = DEFAULT_WIDTH;
	height = DEFAULT_HEIGHT;

	initializeGLFW();
	initializeGLEW();
	
	screenManager.setCurrentScreen(new ScreenCreateCurve(&screenManager));
}

void Engine::initializeGLFW()
{
	glfwSetErrorCallback(global_error_callback);
	
	if(!glfwInit())
	{
		std::cout << "-> GLFW Initialize error" << std::endl;
	}

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

void Engine::initializeGLEW()
{
	window = glfwCreateWindow(width, height, "Fred's rendering engine", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, global_key_callback);
	glfwSetCursorPosCallback(window, global_mouse_callback);
	glfwSetMouseButtonCallback(window, global_mouse_button_callback);

	glfwSetScrollCallback(window, global_scroll_callback);

	glfwSetWindowSizeCallback(window, global_window_size_callback);
	glfwSetFramebufferSizeCallback(window, global_framebuffer_size_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	lastMouseX = width / 2;
	lastMouseY = height / 2;

	glEnable(GL_DEPTH_TEST);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


void Engine::error_callback(int error, const char* description)
{
	std::cout << "Error callback ("<<error<<") "<<description << std::endl;
	puts(description);
}

void Engine::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void Engine::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Screen* screen = screenManager.getCurrentScreen();
	if(screen != nullptr)
	{
		screen->mouse_button_callback(window, button, action, mods);
	}
}
