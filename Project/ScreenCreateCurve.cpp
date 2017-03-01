#include "ScreenCreateCurve.h"
#include <iostream>
#include <string>
#include "ScreenDisplaySweep.h"

ScreenCreateCurve::ScreenCreateCurve(ScreenManager* screenManager):Screen(screenManager)
{
	shader = new Shader("a1Shader.vs", "a1Shader.fs");
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	updateRendering(profile_curve);

	
}

void ScreenCreateCurve::tick()
{
	
}
void ScreenCreateCurve::render()
{
		glClearColor(255.0f, 255.0f, 255.0f, 255.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->use();

		std::vector<glm::vec3>* curve = nullptr;
	switch(phase)
	{
	case DISPLAYING_TRAJECTORY_CURVE:
	case CREATING_TRAJECTORY_CURVE:
		curve = &trajectory_curve;
		break;
	case DISPLAYING_PROFILE_CURVE:
	case CREATING_PROFILE_CURVE:
		curve = &profile_curve;
		break;
	}

		glBindVertexArray(VAO);
		glPointSize(4);
		if (curve != nullptr) {
			glDrawArrays(GL_POINTS, 0, curve->size());
			if(phase == DISPLAYING_PROFILE_CURVE)
				glDrawElements(GL_LINES, EBOSize, GL_UNSIGNED_INT, 0);
		}

		glBindVertexArray(0);



}

void ScreenCreateCurve::updateRendering(std::vector<glm::vec3> curve)
{

	if(vboArray!=nullptr)
	{
		delete vboArray;
	}
	if(eboArray!=nullptr)
	{
		delete eboArray;
	}
	int size = curve.size() * 3;
	vboArray = new GLfloat[size];

	int vboIndex = 0;
	for(glm::vec3 vec : curve)
	{
		vboArray[vboIndex++] = vec.x*2 / screenManager->getWidth()-1;
		vboArray[vboIndex++] = 1-(vec.y*2 / screenManager->getHeight());
		vboArray[vboIndex++] = vec.z;
	}
	EBOSize = curve.size() * 2 * sizeof(GLuint);
	eboArray = new GLuint[EBOSize];
	int eboIndex = 0;
	if(curve.size() > 1)
	for(int i=0; i < curve.size()-1; i++)
	{
		eboArray[eboIndex++] = i;
		eboArray[eboIndex++] = i+1;
	}


	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size*sizeof(GLfloat), vboArray, GL_DYNAMIC_DRAW);
	if (curve.size() > 0) 
	{
		

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, EBOSize, eboArray, GL_DYNAMIC_DRAW);
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}
glm::vec3 ScreenCreateCurve::HermineFunction(float u, glm::vec3 p1, glm::vec3 p2, glm::vec3 t1, glm::vec3 t2)
{
	float h1 = 2 * pow(u, 3) - 3 * pow(u, 2) + 1;
	float h2 = -2 * pow(u, 3) + 3 * pow(u, 2);
	float h3 = pow(u, 3) - 2 * pow(u, 2) + u;
	float h4 = pow(u, 3) - pow(u, 2);

	return p1*h1 + p2*h2 + t1*h3 + t2*h4;
}

std::vector<glm::vec3> ScreenCreateCurve::createHermineSpline(std::vector<glm::vec3> points, std::vector<glm::vec3> tangents)
{
	std::vector<glm::vec3> splinePoints;
	int steps = 20;
	for(int i=0; i < points.size()-1; i++)
	{
		glm::vec3 p1 = points.at(i);
		glm::vec3 p2 = points.at(i+1);

		glm::vec3 t1 = tangents.at(i);
		glm::vec3 t2 = tangents.at(i + 1);

		subdivision(&splinePoints, 0, t1, 1,t2, p1, p2, t1, t2, 10);

	}
	std::cout << points.size() << " : " << splinePoints.size();

	return splinePoints;
}

std::vector<glm::vec3> ScreenCreateCurve::createCatmullRomSpline(std::vector<glm::vec3> points)
{
	
	if(points.size()<=1)
	{
		return std::vector<glm::vec3>();
	}

	std::vector<glm::vec3> tangentVectors;
	std::vector<glm::vec3> pointList;

	for(int i = 1; i < points.size()-1; i++)
	{
		glm::vec3 point = points.at(i);

			glm::vec3 prevPoint = points.at(i - 1);
			glm::vec3 nextPoint = points.at(i + 1);

			float s = 0.5;

			glm::vec3 tVector = (prevPoint - nextPoint);
			glm::vec3 tangent = glm::vec3(tVector.x*s, tVector.y*s, tVector.z*s);
			tangentVectors.push_back(-tangent);
			pointList.push_back(point);
	}

	
	return createHermineSpline(pointList, tangentVectors);
}

void ScreenCreateCurve::subdivision(std::vector<glm::vec3>* points, float u0, glm::vec3 u0T, float u1, glm::vec3 u1T, glm::vec3 p1, glm::vec3 p2, glm::vec3 t1, glm::vec3 t2, float angleDif)
{
	float uMid = (u0 + u1) / 2;

	glm::vec3 x0 = HermineFunction(u0,p1,p2,t1,t2);
	glm::vec3 x1 = HermineFunction(u1,p1,p2,t1,t2);

	glm::vec3 midTang = (x0 - x1);

	glm::vec3 tangDifference = u0T - u1T;

	float l1 = glm::length(u0T);
	float l2 = glm::length(u1T);

	float dot = glm::dot(u0T, u1T);

	float angle = glm::degrees(acos(dot/(abs(l1*l2))));

	if(angle > 90 && angle < 270)
	{
		angle -= 180;
	}

	if(abs(angle )> angleDif)
	{
		subdivision(points, u0, u0T, uMid, midTang, p1, p2, t1, t2, angleDif);
		subdivision(points, uMid, midTang, u1, u1T, p1, p2, t1, t2, angleDif);
	}else
	{
		points->push_back(x0);
		points->push_back(x1);
	}
}


void ScreenCreateCurve::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		switch (phase)
		{
		case CREATING_PROFILE_CURVE:
			profile_curve = createCatmullRomSpline(profile_curve);
			updateRendering(profile_curve);
			phase = DISPLAYING_PROFILE_CURVE;
			break;
		case CREATING_TRAJECTORY_CURVE:
			trajectory_curve = createCatmullRomSpline(trajectory_curve);
			updateRendering(trajectory_curve);
			phase = DISPLAYING_TRAJECTORY_CURVE;
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
			screenManager->setCurrentScreen(new ScreenDisplaySweep(screenManager, profile_curve, trajectory_curve, 0));
			break;
		}
	}
}
void ScreenCreateCurve::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if (phase == DISPLAYING_PROFILE_CURVE)
			phase = CREATING_TRAJECTORY_CURVE;

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout << "Clicked on screen at " + std::to_string(xpos) + "," + std::to_string(ypos) << std::endl;
		switch(phase)
		{
		case CREATING_PROFILE_CURVE:
			profile_curve.push_back(glm::vec3(xpos, ypos, 0));
			updateRendering(profile_curve);
			break;
		case CREATING_TRAJECTORY_CURVE:
			trajectory_curve.push_back(glm::vec3(xpos, ypos, 0));
			updateRendering(trajectory_curve);
			break;
		}
	}
}

void ScreenCreateCurve::window_size_callback(GLFWwindow* window, int width, int height)
{
	switch (phase)
	{
	case DISPLAYING_PROFILE_CURVE:
	case CREATING_PROFILE_CURVE:
		updateRendering(profile_curve);
		break;
	case DISPLAYING_TRAJECTORY_CURVE:
	case CREATING_TRAJECTORY_CURVE:
		updateRendering(trajectory_curve);
		break;
	}
}
