#ifndef SHADERS_H
#define SHADERS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <string>

class Shader
{
private:
	int m_programId{};
public:
	Shader(const char* vertexPath, const char* fragmentPath);

	void setMatrixUniform(const char* name, glm::mat4 matrix) const
	{
		int uniformLocation = glGetUniformLocation(m_programId, name);
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	void setIntUniform(const char* name, int value) const
	{
		int uniformLocation = glGetUniformLocation(m_programId, name);
		glUniform1i(uniformLocation,value);
	}
	
	void useShader() const
	{
		glUseProgram(m_programId);
	}
	void uniformBufferBinding(const char* name,int bindPoint)
	{
		unsigned int uniformBlock = glGetUniformBlockIndex(m_programId, name);
		glUniformBlockBinding(m_programId, uniformBlock, bindPoint);
	}
	


};

Shader::Shader(const char* vertexPath,const char* fragmentPath)
{	
	unsigned int vertexID{};
	unsigned int fragmentID{};


	std::ifstream vertexFile{vertexPath};
	if (!vertexFile)
	{
		std::cout << "ERROR::COULDNT::OPEN::SHADER::FILE\n";
	}
	std::ifstream fragmentFile{fragmentPath};
	if (!fragmentFile)
	{
		std::cout << "ERROR::COULDNT::OPEN::SHADER::FILE\n";
	}

	//Ladowanie kodu shaderu z pliku
	std::string vertexCode{};
	std::string fragmentCode{};
	std::string line{};
	
	while (std::getline(vertexFile, line))
	{
		vertexCode += line + "\n";
	}
	
	
	while (std::getline(fragmentFile, line))
	{
		fragmentCode += line + "\n";
	}
	
	vertexID = glCreateShader(GL_VERTEX_SHADER);
	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vertexSource;
	vertexSource = vertexCode.c_str();
	glShaderSource(vertexID, 1, &vertexSource, NULL);
	glCompileShader(vertexID);
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		std::cout << infoLog << "\n";
	}

	const char* fragmentSource;
	fragmentSource = fragmentCode.c_str();
	glShaderSource(fragmentID, 1, &fragmentSource, NULL);
	glCompileShader(fragmentID);
	int  success1;
	char infoLog1[512];
	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success1);
	if (!success1)
	{
		glGetShaderInfoLog(fragmentID, 512, NULL, infoLog1);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog1 << std::endl;
		std::cout << infoLog1 << "\n";
	}

	m_programId = glCreateProgram();
	glAttachShader(m_programId, vertexID);
	glAttachShader(m_programId, fragmentID);
	glLinkProgram(m_programId);



	

}
#endif 
