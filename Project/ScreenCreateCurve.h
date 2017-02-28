#pragma once
#include "Screen.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "VaoObject.h"

class ScreenCreateCurve:public Screen
{
private:
	enum CURRENT_PHASE
	{
		CREATING_PROFILE_CURVE,
		CREATING_TRAJECTORY_CURVE
	};
	CURRENT_PHASE phase = CREATING_PROFILE_CURVE;

	std::vector<glm::vec3> profile_curve;
	std::vector<glm::vec3> trajectory_curve;

	Shader* shader;

	
	void updateRendering();

	GLuint VAO;
	GLuint VBO;

	GLfloat* vboArray;

public:
	ScreenCreateCurve(ScreenManager*);

	

	void tick();
	void render();

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);



};
