#include "ScreenCreateCurve.h"
#include <iostream>
#include <string>

ScreenCreateCurve::ScreenCreateCurve(ScreenManager* screenManager):Screen(screenManager)
{
	shader = new Shader("a1Shader.vs", "a1Shader.fs");
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(profile_curve.size() * 3), vboArray, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	updateRendering();
}

void ScreenCreateCurve::tick()
{
	
}
void ScreenCreateCurve::render()
{
	if (profile_curve.size()>0) {
		glClearColor(255.0f, 255.0f, 255.0f, 255.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->use();

		glBindVertexArray(VAO);
		glPointSize(10);
		glDrawArrays(GL_POINTS, 1, profile_curve.size());

		glBindVertexArray(0);

	}

}

void ScreenCreateCurve::updateRendering()
{

	if(vboArray!=nullptr)
	{
		delete vboArray;
	}
	int size = profile_curve.size() * 3;
	vboArray = new GLfloat[size];
	int vboIndex = 0;
	for(glm::vec3 vec : profile_curve)
	{
		vboArray[vboIndex++] = vec.x*2 / screenManager->getWidth()-1;
		vboArray[vboIndex++] = 1-(vec.y*2 / screenManager->getHeight());
		vboArray[vboIndex++] = vec.z;
	}

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size*sizeof(GLfloat), vboArray, GL_DYNAMIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}


void ScreenCreateCurve::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	
}
void ScreenCreateCurve::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout << "Clicked on screen at " + std::to_string(xpos) + "," + std::to_string(ypos) << std::endl;;
		switch(phase)
		{
		case CREATING_PROFILE_CURVE:
			profile_curve.push_back(glm::vec3(xpos, ypos, 0));
			updateRendering();
			break;
		case CREATING_TRAJECTORY_CURVE:
			trajectory_curve.push_back(glm::vec3(xpos, ypos, 0));
			updateRendering();
			break;
		}
	}
}

