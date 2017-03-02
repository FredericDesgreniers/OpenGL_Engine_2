#pragma once
#include "Screen.h"
#include <vector>
#include <glm/detail/type_vec3.hpp>
#include "Camera.h"
#include "Shader.h"

class ScreenDisplaySweep:public Screen
{
public:
	ScreenDisplaySweep(ScreenManager* screenManager, std::vector<glm::vec3> profileCurve, std::vector<glm::vec3> trajectoryCurve, int num);

	void render();
	void tick();
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	

	glm::vec3 rotation;

private:
	bool firstMouse = false;
	GLFWwindow* window;
	enum SWEEP_TYPE
	{
		TRANSLATE,
		ROTATE
	};
	SWEEP_TYPE sweep;
	int span;
	GLuint VAO, VBO, EBO;

	GLfloat* vboArray;
	GLuint* eboArray;

	int size, indicesSize;

	//delta time
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	void calculateDeltaTime();

	GLfloat lastMouseX, lastMouseY;

	void doRotateSweep();
	void doTranslateSweep();

	Camera camera;

	Shader* shader;

	std::vector<glm::vec3> profileCurve, trajectoryCurve;
};
