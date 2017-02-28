#pragma once
#include <GL/glew.h>
#include <glm/detail/type_vec3.hpp>

class VaoObject
{
public:
	VaoObject(int vboSize, int eboSize);

	void addToVBO(GLfloat vertice);
	void addToVBO(glm::vec3 vertice);
	
	void addToEBO(int point);
	void addTriangleToEBO(int point1, int point2, int point3);
	
	void addTrianglesFromPointsToEBO(int point1, int point2, int point3, int point4);

	void setupVao();

	void printVBO();
	void printEBO();

	int getCurrentVboIndex();
	int getCurrentEboIndex();

	GLuint getVAO();
	GLuint getEBO();
	GLuint getVBO();

	int getEboSize();
private:
	GLuint VAO, VBO, EBO = -1;

	int vboSize;
	int eboSize;

	int vboIndex = 0;
	int eboIndex = 0;
	
	GLfloat* vboArray;
	GLuint* eboArray;



};
