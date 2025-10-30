#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "Chunk.h"

class Player
{
private:
	glm::vec3* playerPos{};
	
public:
	Player(Camera* camera)
		:playerPos{&camera->cameraPos}
	{

	}	
	void substractFromPlayerPos(glm::vec3 newPos)
	{
		playerPos->y = playerPos->y-newPos.y;
	}
	void addToPlayerPos(glm::vec3 newPos)
	{
		playerPos->y = playerPos->y + newPos.y;
	}
	void addBlock(glm::vec3 cameraPos, glm::vec3 cameraFront, Chunk* chunk,int type);
	void removeBlock(glm::vec3 cameraPos, glm::vec3 cameraFront, Chunk* chunk);
	glm::vec3 getPlayerPos();

};
void Player::addBlock(glm::vec3 cameraPos, glm::vec3 cameraFront, Chunk* chunk,int type)
{
	std::vector<int> possibleIndices{};
	bool firstSolidFound = false;
	float offset = 0.1;
	glm::vec3 nextCheckPos{};

	for (int y = 0; y < 1000; y++)
	{
		if (firstSolidFound)
		{
			break;
		}
		offset = offset + 0.1f;
		nextCheckPos = cameraPos + cameraFront * offset;
		for (int i = 0; i < chunk->getChunkSize(); i++)
		{



			if (
				(chunk->getBlockCenter(i).x - 0.5f < nextCheckPos.x && chunk->getBlockCenter(i).x + 0.5f > nextCheckPos.x) &&
				(chunk->getBlockCenter(i).y - 0.5f < nextCheckPos.y && chunk->getBlockCenter(i).y + 0.5f > nextCheckPos.y) &&
				(chunk->getBlockCenter(i).z - 0.5f < nextCheckPos.z && chunk->getBlockCenter(i).z + 0.5f > nextCheckPos.z)
				)
			{

				possibleIndices.push_back(i);
				if (chunk->getBlockType(i) != 0)
				{
					firstSolidFound = true;
					chunk->setclickedBlockIndex(i);
					break;
				}

			}
			//teraz musze zorbic tak aby ppo dodnaiu bloku nie zminial sie porzenid dodany spowortem na lave

		}




	}
	int length = possibleIndices.size();
	if (length - 2 > -1)
	{
		chunk->setBlockType(possibleIndices, type);
	}




}



void Player::removeBlock(glm::vec3 cameraPos, glm::vec3 cameraFront, Chunk* chunk)
{
	int foundIndice{ -1 };
	bool found = false;
	float offset = 0.1;
	glm::vec3 nextCheckPos{};
	for (int y = 0; y < 1000; y++)
	{
		if (found)
		{
			break;
		}
		offset = offset + 0.1f;
		nextCheckPos = cameraPos + cameraFront * offset;
		for (int i = 0; i < chunk->getChunkSize(); i++)
		{



			if (
				(chunk->getBlockCenter(i).x - 0.5f < nextCheckPos.x && chunk->getBlockCenter(i).x + 0.5f > nextCheckPos.x) &&
				(chunk->getBlockCenter(i).y - 0.5f < nextCheckPos.y && chunk->getBlockCenter(i).y + 0.5f > nextCheckPos.y) &&
				(chunk->getBlockCenter(i).z - 0.5f < nextCheckPos.z && chunk->getBlockCenter(i).z + 0.5f > nextCheckPos.z)
				)
			{

				if (chunk->getBlockType(i) != 0)
				{
					foundIndice = i;
					found = true;
					break;
				}



			}

		}




	}

	if (foundIndice > -1)
	{
		chunk->setBlockType(foundIndice, 0);
	}
}

glm::vec3 Player::getPlayerPos() {
	return *playerPos;
}
