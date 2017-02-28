#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader
{
public:
	GLuint program;

	Shader(const char* vertexPath, const char* fragmentPath);

	void use();

private:
	std::string readShaderCodeFromFile(std::string path);
};
