#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
class UniformBuffer
{
private:
	unsigned int m_id{};
public:
	UniformBuffer(int amount, int point, int offset)
	{
		glGenBuffers(1, &m_id);
		bindUniformBuffer();
		glBufferData(GL_UNIFORM_BUFFER, amount * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
		unbindUniformBuffer();
		glBindBufferRange(GL_UNIFORM_BUFFER, point, m_id, offset, amount * sizeof(glm::mat4));
	}
	void loadDataToUboMat4(int offset, glm::mat4 matrix)
	{
		bindUniformBuffer();
		glBufferSubData(GL_UNIFORM_BUFFER, offset * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(matrix));
		unbindUniformBuffer();

	}
	void bindUniformBuffer()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_id);
	}
	void unbindUniformBuffer()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	
};
