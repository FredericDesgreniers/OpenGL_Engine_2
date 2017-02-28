#include "VaoObject.h"
#include <iostream>

VaoObject::VaoObject(int vboSize, int eboSize):vboSize(vboSize), eboSize(eboSize)
{
	vboArray = new GLfloat[vboSize];
	eboArray = new GLuint[eboSize];
}

void VaoObject::addToVBO(GLfloat vertice)
{
	
	vboArray[vboIndex++] = vertice;
}

void VaoObject::addToVBO(glm::vec3 vertice)
{
	addToVBO(vertice.x);
	addToVBO(vertice.y);
	addToVBO(vertice.z);
}


void VaoObject::addToEBO(int point)
{
	eboArray[eboIndex++] = point;
}

void VaoObject::addTriangleToEBO(int point1, int point2, int point3)
{
	addToEBO(point1);
	addToEBO(point2);
	addToEBO(point3);
}

void VaoObject::addTrianglesFromPointsToEBO(int point1, int point2, int point3, int point4)
{
	addTriangleToEBO(point1, point2, point4);
	addTriangleToEBO(point1, point3, point4);
}

void VaoObject::setupVao()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//bind the vertices array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vboArray)*vboSize, vboArray, GL_STATIC_DRAW);

	//bind indices array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(eboArray)*vboSize, eboArray, GL_STATIC_DRAW);

	//tell opengl to look at them in pairs of 3 (x,y,z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void VaoObject::printVBO()
{
	std::cout << "VBO size: " << vboSize << std::endl;
	int c = 0;
	for (int i = 0; i < vboSize; i++)
	{
		printf("%f ", vboArray[i]);
		c++;
		if (c > 2)
		{
			c = 0;
			std::cout << std::endl;
		}
	}
}


void VaoObject::printEBO()
{
	int c = 0;
	std::cout << "EBO size: " << eboSize << std::endl;
	for (int i = 0; i < eboSize; i++)
	{
		printf("%d ", eboArray[i]);
		c++;
		if (c > 2)
		{
			c = 0;
			std::cout << std::endl;
		}
	}
}

int VaoObject::getCurrentEboIndex()
{
	return eboIndex;
}


int VaoObject::getCurrentVboIndex()
{
	return vboIndex;
}

GLuint VaoObject::getVAO()
{
	return VAO;
}

int VaoObject::getEboSize()
{
	return eboSize;
}


GLuint VaoObject::getEBO()
{
	return EBO;
}

GLuint VaoObject::getVBO()
{
	return VBO;
}
