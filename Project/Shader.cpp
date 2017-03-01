#include "Shader.h"
#include <GL/glew.h>


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::cout << "Loading shader for " << vertexPath << " "<<fragmentPath<< std::endl;
	std::string vertexCode;
	std::string fragmentCode;



	try
	{
		vertexCode = readShaderCodeFromFile(vertexPath);
		fragmentCode = readShaderCodeFromFile(fragmentPath);
		std::cout << vertexCode << std::endl;
		

	}catch(std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

}

std::string Shader::readShaderCodeFromFile(std::string path)
{
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::badbit);

	shaderFile.open(path);

	std::stringstream shaderStream;

	shaderStream << shaderFile.rdbuf();

	shaderFile.close();

	return shaderStream.str();
	
}

void Shader::use()
{
	glUseProgram(program);
}

