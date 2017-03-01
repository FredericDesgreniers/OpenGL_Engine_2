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
		DISPLAYING_PROFILE_CURVE,
		CREATING_TRAJECTORY_CURVE,
		DISPLAYING_TRAJECTORY_CURVE
	};

	enum SWEEP_TYPE
	{
		UNDEFINED,
		TRANSLATE,
		ROTATE
	};
	SWEEP_TYPE type = UNDEFINED;

	CURRENT_PHASE phase = CREATING_PROFILE_CURVE;

	std::vector<glm::vec3> profile_curve;
	std::vector<glm::vec3> trajectory_curve;

	Shader* shader;

	
	void updateRendering(std::vector<glm::vec3>);

	void subdivision(std::vector<glm::vec3>*,float u0, glm::vec3 u0T, float u1, glm::vec3 u1T, glm::vec3 p1, glm::vec3 p2, glm::vec3 t1, glm::vec3 t2, float maxLineLength);

	std::vector<glm::vec3> normalizeCurve(std::vector<glm::vec3>);

	glm::vec3 HermineFunction(float u, glm::vec3 p1, glm::vec3 p2, glm::vec3 t1, glm::vec3 t2);

	std::vector<glm::vec3> createCatmullRomSpline(std::vector<glm::vec3>);
	std::vector<glm::vec3> createHermineSpline(std::vector<glm::vec3>, std::vector<glm::vec3>);

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	int EBOSize;

	GLfloat* vboArray;
	GLuint* eboArray;

public:
	ScreenCreateCurve(ScreenManager*);

	
	

	void tick();
	void render();

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	void window_size_callback(GLFWwindow* window, int width, int height);

};
