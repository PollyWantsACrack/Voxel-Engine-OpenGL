#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "stb_image.h"

#include <vector>
class Vao
{
private:
	GLuint vaoID, vboID, texVboId;
public:
	Vao(std::vector<float>& vertices)
	{
		
		glGenVertexArrays(1, &vaoID);
		glGenBuffers(1, &vboID);
		glBindVertexArray(vaoID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
	}
	void bindVAO() const
	{
		glBindVertexArray(vaoID);
	}
	void bindVBO() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
	}
	void unbindVAO() const
	{
		glBindVertexArray(0);
	}
	void setVertexAttribPointer(int location,int numberOfValuesPerAttribute, int stride, int offset)
	{
		glVertexAttribPointer(location, numberOfValuesPerAttribute, GL_FLOAT, GL_FALSE, stride * sizeof(float), (GLvoid*)(offset * sizeof(GLfloat)));
		glEnableVertexAttribArray(location);
	}
};
