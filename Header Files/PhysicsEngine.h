#ifndef PHYSICS_H
#define PHYSICS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <vector>
#include "Chunk.h"
#include "Camera.h"
#include "Player.h"
class Camera;

class PhysicsEngine
{
private:
	float currentTime{ 0.0f };
	bool playerOnGround{ true };
	bool playerDuringJump{ false };
	bool jumpProcesStarted{ false };
	bool collisionDetected{ false };
	float lastVelocity{ 0.005f };
	float lastTime{ 0.0f };
	float elapsedTime{ 0.0f };
	enum KeyDirection
	{
		W,
		S,
	};
public:
	PhysicsEngine() = default;
	KeyDirection getDir(char c)
	{
		switch (c)
		{
		case 'W':
			return  W;
			break;
		case 'S':
			return  S;
			break;
		}
			

	}
	bool getOngroundStatus()
	{
		return playerOnGround;
	}
	void updateTime(float time)
	{
		currentTime = time;
	}
	float getCurrentTime()
	{
		return currentTime;
	}
	void startJump()
	{
		if (!jumpProcesStarted)
		{
			playerDuringJump = true;
			lastTime = currentTime;
		}
		
	}
	void isPlayerOnGround(Camera* camera, Chunk* chunk, Player* player);
	glm::vec3 checkCollision(Camera* camera, Chunk* chunk, KeyDirection dir);
	void applyGravity(Player* player);
	void jump(Player* player, Camera* camera, Chunk* chunk);

	
	
};
void PhysicsEngine::jump(Player* player, Camera* camera, Chunk* chunk) {
	bool subtracted = false;
	if (playerDuringJump)
	{
		elapsedTime = currentTime - lastTime;
		if (currentTime - lastTime < 0.25f)
		{
			float offset = -8 * (elapsedTime - 0) * (elapsedTime - 2.5);
			glm::vec3 predictedPositionY = camera->getCameraPos();
			predictedPositionY.y = camera->getCameraPos().y - camera->getCameraSpeed() - 1.51f;

			if (!subtracted)
			{
				player->addToPlayerPos(glm::vec3(0.0f, offset / 240, 0.0f));
				subtracted = true;
			}
			else if (subtracted)
			{
				
			}
			jumpProcesStarted = true;
			
		}
		else {
			playerDuringJump = false;
			lastTime = 0.0f;
			elapsedTime = 0.0f;
		}
	}	
}


void PhysicsEngine::applyGravity(Player* player)
{
	
	if (!playerDuringJump) {
		if (!playerOnGround)
		{

			if (lastVelocity >= 0.01f)
			{
				lastVelocity = lastVelocity;
			}
			else {
				lastVelocity = lastVelocity * 1.025f;
			}
			player->substractFromPlayerPos(glm::vec3(0.0f, lastVelocity, 0.01f));
		}
		else {
			
			lastVelocity = 0.001f;
		}
	}
	
}
void PhysicsEngine::isPlayerOnGround(Camera* camera, Chunk* chunk, Player* player)
{
		
		glm::vec3 predictedPositionY = camera->getCameraPos();
		predictedPositionY.y = camera->getCameraPos().y - camera->getCameraSpeed() - 1.51f;

		for (int i = 0; i < chunk->getChunkSize(); i++)
		{
			if (chunk->getBlockType(i) == 0)
			{
				continue;
			}
			if (
				(predictedPositionY.x > chunk->getBlockCenter(i).x - 0.65f && predictedPositionY.x < chunk->getBlockCenter(i).x + 0.65f) &&
				(predictedPositionY.y > chunk->getBlockCenter(i).y - 0.5f && predictedPositionY.y < chunk->getBlockCenter(i).y + 0.5f) &&
				(predictedPositionY.z > chunk->getBlockCenter(i).z - 0.65f && predictedPositionY.z < chunk->getBlockCenter(i).z + 0.65f)
				)
			{
				playerOnGround = true;
				jumpProcesStarted = false;
				break;
			}
			else {
				playerOnGround = false;
				
			}
		}
}

glm::vec3 PhysicsEngine::checkCollision(Camera* camera, Chunk* chunk, KeyDirection dir)
{
	glm::vec3 result{ 1.0f, 1.0f, 1.0f };
	glm::vec3 predictedPositionX;
	glm::vec3 predictedPositionY;
	glm::vec3 predictedPositionZ;
	float speed = camera->getCameraSpeed();
	if (dir == W)
	{
		predictedPositionX = camera->getCameraPos() + speed * glm::vec3(camera->getCameraFront().x, 0, 0);
		predictedPositionY = camera->getCameraPos() + speed * glm::vec3(0, camera->getCameraFront().y, 0);
		predictedPositionZ = camera->getCameraPos() + speed * glm::vec3(0, 0, camera->getCameraFront().z);
	}
	else if (dir == S)
	{
		predictedPositionX = camera->getCameraPos() - speed * glm::vec3(camera->getCameraFront().x, 0, 0);
		predictedPositionY = camera->getCameraPos() - speed * glm::vec3(0, camera->getCameraFront().y, 0);
		predictedPositionZ = camera->getCameraPos() - speed * glm::vec3(0, 0, camera->getCameraFront().z);
	}

	bool collisionX = false;
	bool collisionY = false;
	bool collisionZ = false;
	for (int i = 0; i < chunk->getChunkSize(); i++)
	{
		if (chunk->getBlockType(i) == 0)
		{
			continue;
		}


		if (
			(predictedPositionX.x > chunk->getBlockCenter(i).x - 0.65f && predictedPositionX.x < chunk->getBlockCenter(i).x + 0.65f) &&
			(predictedPositionX.y > chunk->getBlockCenter(i).y - 0.5f && predictedPositionX.y < chunk->getBlockCenter(i).y + 0.5f + 1.5f) &&
			(predictedPositionX.z > chunk->getBlockCenter(i).z - 0.65f && predictedPositionX.z < chunk->getBlockCenter(i).z + 0.65f)
			)
		{
			collisionX = true;
			std::cout << "collision X\n";
			result.x = 0.0f;
		}
		else if (!collisionX) {
			result.x = 1.0f;

		}
		if (
			(predictedPositionY.x > chunk->getBlockCenter(i).x - 0.65f && predictedPositionY.x < chunk->getBlockCenter(i).x + 0.65f) &&
			(predictedPositionY.y > chunk->getBlockCenter(i).y - 0.5f && predictedPositionY.y < chunk->getBlockCenter(i).y + 0.5f + 1.5f) &&
			(predictedPositionY.z > chunk->getBlockCenter(i).z - 0.65f && predictedPositionY.z < chunk->getBlockCenter(i).z + 0.65f)
			)
		{
			collisionY = true;
			std::cout << "collision Y\n";
			result.y = 0.0f;
		}
		else if (!collisionY) {
			result.y = 1.0f;

		}
		if (
			(predictedPositionZ.x > chunk->getBlockCenter(i).x - 0.65f && predictedPositionZ.x < chunk->getBlockCenter(i).x + 0.65f) &&
			(predictedPositionZ.y > chunk->getBlockCenter(i).y - 0.5f && predictedPositionZ.y < chunk->getBlockCenter(i).y + 0.5f + 1.5f) &&
			(predictedPositionZ.z > chunk->getBlockCenter(i).z - 0.65f && predictedPositionZ.z < chunk->getBlockCenter(i).z + 0.65f)
			)
		{
			collisionZ = true;
			std::cout << "collision Z\n";
			result.z = 0.0f;
		}
		else if (!collisionZ) {
			result.z = 1.0f;

		}

	}

	return result;
}
#endif 