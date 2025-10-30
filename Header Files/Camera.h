#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "Shader.h"
#include <vector>
#include "Chunk.h"
#include <cmath>

class Chunk;

class Camera
{
private:
	bool firstmouse = true;
	float yaw{ 270.0f };
	float pitch{ 0.0f };
	float sens = 0.05f;
	float cameraSpeed = 0.1f;


	double lastXpos{};
	double lastYpos{};

	glm::vec3 cameraPos{ 0.0f,66.0f,3.0f };
	glm::vec3 cameraFront{ 0.0f,0.0f,-1.0f };
	glm::vec3 cameraUp{ 0.0f,1.0f,0.0f };
	glm::mat4 lookAt = glm::mat4(1.0f);
public:
	friend class Player;

	enum KeyDirection
	{
		W,
		S,
		A,
		D,
		SPACE,
		LSHIFT,
		KEY_2,
		KEY_3
	};

	Camera() = default;

	glm::mat4 getLookAt() const
	{
		return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}
	glm::vec3 getCameraPos() const
	{
		return cameraPos;
	}
	glm::vec3 getCameraFront() const
	{
		return cameraFront;
	}
	float getCameraSpeed() const
	{
		return cameraSpeed;
	}
	void setCameraPos(float x)
	{
		cameraPos.x = x;
	}
	void setCameraPosY(float y)
	{
		cameraPos.y = y;
	}
	void updateVectorKey(KeyDirection dir, Chunk* chunk,glm::vec3 axis);
	

	void updateVectorMouse(double xpos, double ypos)
	{
		if (firstmouse)
		{
			lastXpos = xpos;
			lastYpos = ypos;
			firstmouse = false;
		}

		float sens = 0.05f;
		float offsetX = xpos - lastXpos;
		float offsetY = ypos - lastYpos;
		lastXpos = xpos;
		lastYpos = ypos;

		yaw += offsetX * sens;
		pitch -= offsetY * sens;

		if (pitch >= 89.0f)
		{
			pitch = 89.0f;
		}
		else if (pitch <= -89.0f)
		{
			pitch = -89.0f;
		}

		glm::vec3 direction(0.0f, 0.0f, 0.0f);
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = normalize(direction);

	}

};
void Camera::updateVectorKey(KeyDirection dir, Chunk* chunk,glm::vec3 axis)
{
	switch (dir)
	{

	case W:
	{
		glm::vec3 newPos = cameraPos;
		glm::vec3 normalizedCameraFront;
		normalizedCameraFront = cameraFront;
		normalizedCameraFront.y = 0.0f;
		float magnitude = glm::sqrt((normalizedCameraFront.x * normalizedCameraFront.x) + (normalizedCameraFront.y * normalizedCameraFront.y) + (normalizedCameraFront.z * normalizedCameraFront.z));
		normalizedCameraFront.x = normalizedCameraFront.x / magnitude;
		normalizedCameraFront.y = normalizedCameraFront.y / magnitude;
		normalizedCameraFront.z = normalizedCameraFront.z / magnitude;

		glm::vec3 predictedPositionX = cameraPos + cameraSpeed * glm::vec3(cameraFront.x, 0, 0);
		glm::vec3 predictedPositionY = cameraPos + cameraSpeed * glm::vec3(0, cameraFront.y, 0);
		glm::vec3 predictedPositionZ = cameraPos + cameraSpeed * glm::vec3(0, 0, cameraFront.z);

		if (axis.x != 0)
		{
			newPos.x = predictedPositionX.x;
		}
		if (axis.y != 0)
		{
			newPos.y = cameraPos.y;
		}
		if (axis.z != 0)
		{
			newPos.z = predictedPositionZ.z;
		}

		cameraPos = newPos;
		break;
		
	}

	case S:
	{
		glm::vec3 newPos = cameraPos;
		glm::vec3 normalizedCameraFront;
		normalizedCameraFront = cameraFront;
		normalizedCameraFront.y = 0.0f;
		glm::normalize(normalizedCameraFront);
		glm::vec3 predictedPositionX = cameraPos - cameraSpeed * glm::vec3(cameraFront.x, 0, 0);
		glm::vec3 predictedPositionY = cameraPos - cameraSpeed * glm::vec3(0, cameraFront.y, 0);
		glm::vec3 predictedPositionZ = cameraPos - cameraSpeed * glm::vec3(0, 0, cameraFront.z);

		if (axis.x != 0)
		{
			newPos.x = predictedPositionX.x;
		}
		if (axis.y != 0)
		{
			newPos.y = cameraPos.y;
		}
		if (axis.z != 0)
		{
			newPos.z = predictedPositionZ.z;
		}

		cameraPos = newPos;
		break;

	}
	case A:
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * -cameraSpeed;
		break;
	case D:
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		break;
	case SPACE:
		cameraPos += cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
		break;
	case LSHIFT:
		cameraPos -= cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
		break;
	case KEY_2:
		cameraSpeed *= 1.1;
		break;
	case KEY_3:
		cameraSpeed /= 1.1;
		break;

	}
}
#endif 