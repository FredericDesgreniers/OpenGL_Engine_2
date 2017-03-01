#include "ScreenDisplaySweep.h"
#include "Engine.h"
void global_mouse_callback1(GLFWwindow* window, double xpos, double ypos)
{
	Engine::getInstance()->mouse_callback(window, xpos, ypos);
}
void global_window_size_callback1(GLFWwindow* window, int width, int height)
{
	Engine::getInstance()->window_size_callback(window, width, height);
}
void global_error_callback1(int error, const char* description)
{
	Engine::getInstance()->error_callback(error, description);
}
ScreenDisplaySweep::ScreenDisplaySweep(ScreenManager* screenManager, std::vector<glm::vec3> profileCurve, std::vector<glm::vec3> trajectoryCurve, int num):Screen(screenManager),  profileCurve(profileCurve), trajectoryCurve(trajectoryCurve)
{
	window = glfwCreateWindow(500, 500, "Fred's rendering engine", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	shader = new Shader("a1Shader1.vs", "a1Shader1.fs");

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glfwSetErrorCallback(global_error_callback1);
	glfwSetCursorPosCallback(window, global_mouse_callback1);
	glfwSetWindowSizeCallback(window, global_window_size_callback1);
	glfwMakeContextCurrent(window);



	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);

	Engine::getInstance()->window = window;

	if (num > 0) {
		sweep = ROTATE;
		doRotateSweep();
	}
	else
	{
		sweep = TRANSLATE;
		doTranslateSweep();
	}


}
void ScreenDisplaySweep::tick()
{
	
}

void ScreenDisplaySweep::render()
{
	glClearColor(255.0f, 255.0f, 255.0f, 255.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view;
	view = camera.getViewMatrix();

	glm::mat4 projection;
	projection = glm::perspective(camera.zoom, GLfloat(screenManager->getWidth()) / GLfloat(screenManager->getHeight()), 0.1f, 1000.0f);
	
	shader->use();

	GLuint modelLoc = glGetUniformLocation(shader->program, "model");

	GLuint viewLoc = glGetUniformLocation(shader->program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	GLuint projectionLoc = glGetUniformLocation(shader->program, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	

	glm::mat4 model;

	model = glm::translate(model, glm::vec3(rotation.x, rotation.y, rotation.z));
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void ScreenDisplaySweep::doRotateSweep()
{
	
}
void ScreenDisplaySweep::doTranslateSweep()
{
	//get the size of the vertices array. 
	size = profileCurve.size() * trajectoryCurve.size() * 3;

	vboArray = new GLfloat[size];

	//number of indices (2 triangles per "square")
	indicesSize = (profileCurve.size() - 1) * (trajectoryCurve.size() - 1) * 2 * 3;
	eboArray = new GLuint[indicesSize];

	//indice counter
	int iIndex = 0;
	//vertex counter
	int index = 0;

	//go through every vector on the profile curve
	for (int p = 0; p < profileCurve.size(); p++)
	{
		glm::vec3 pVec = profileCurve.at(p);
		//for each profile point, go through the trajectory vectors
		for (int t = 0; t < trajectoryCurve.size(); t++)
		{
			float height = (float(p) / float(profileCurve.size()));
			glm::vec3 tVec = trajectoryCurve.at(t);


			//translate the profile vector by the trajectory vector and add it to the vector array
			vboArray[index] = pVec.x + tVec.x;
			vboArray[index + 1] = pVec.y + tVec.y;
			vboArray[index + 2] = pVec.z + tVec.z;

			index += 3;

			//if we're not in a bottom / left row, find the indices that compose the two triangles
			//the two triangles compose the square that is to the bottom-left of the current vector
			if (t > 0 && p > 0)
			{
				//first triangle
				eboArray[iIndex] = index / 6 - 1;
				eboArray[iIndex + 1] = index / 6 - 1 - 1;
				eboArray[iIndex + 2] = index / 6 - 1 - trajectoryCurve.size() - 1;
				iIndex += 3;

				//second triangle
				eboArray[iIndex] = index / 6 - 1;
				eboArray[iIndex + 1] = index / 6 - trajectoryCurve.size() - 1;
				eboArray[iIndex + 2] = index / 6 - 1 - trajectoryCurve.size() - 1;
				iIndex += 3;

			}

		}
		
	}
	std::cout << GL_MAX_ELEMENTS_VERTICES << "," << GL_MAX_ELEMENTS_INDICES;

	//bind the VAO/VBO/EBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//bind the vertices array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*size, vboArray, GL_STATIC_DRAW);

	//bind indices array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(eboArray)*indicesSize, eboArray, GL_STATIC_DRAW);

	//tell opengl to look at them in pairs of 3 (x,y,z)
	//TODO add color here, so (x,y,z,r,g,b)?
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void ScreenDisplaySweep::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastMouseX = xpos;
		lastMouseY = ypos;
		firstMouse = false;
	}
	GLfloat xoffset = xpos - lastMouseX;
	GLfloat yoffset = ypos - lastMouseY;

	lastMouseX = xpos;
	lastMouseY = ypos;

	camera.processMouseMovement(xoffset, yoffset);
}
