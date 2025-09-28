#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Vao.h"
#include "ChunkLoader.h"
#include <vector>
#include "UniformBuffer.h"
#include "Block.h"


class Chunk 
{
private:
	
	std::vector<Block> blocks;
	int offset = 1;

public:
	
	std::vector<float> vertices;
	
	Chunk()
	: blocks(16 * 16 * 128)
	{
		vertices.reserve(blocks.size() * 36 * 5);
		for (int i = 0; i < blocks.size(); i++)
		{

			if (i < 16 * 16 * 128 / 2)
			{
				blocks[i] = (Block{ 1 });
			}
			else {
				blocks[i] = (Block{ 0 });
			}
						
		}
		

	}
	friend class Camera;
	void generateVerticesOfChunk();
	void removeTargetBlock(glm::vec3 cameraPos, glm::vec3 cameraFront);
	void addBlock(glm::vec3 cameraPos, glm::vec3 cameraFront);
	
};
void Chunk::addBlock(glm::vec3 cameraPos, glm::vec3 cameraFront)
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
		for (int i = 0; i < blocks.size(); i++)
		{



			if (
				(blocks[i].cubeCenter.x - 0.5f < nextCheckPos.x && blocks[i].cubeCenter.x + 0.5f > nextCheckPos.x) &&
				(blocks[i].cubeCenter.y - 0.5f < nextCheckPos.y && blocks[i].cubeCenter.y + 0.5f > nextCheckPos.y) &&
				(blocks[i].cubeCenter.z - 0.5f < nextCheckPos.z && blocks[i].cubeCenter.z + 0.5f > nextCheckPos.z) 
				)
			{
				
				possibleIndices.push_back(i);
				if (blocks[i].blockType == 1)
				{
					firstSolidFound = true;
					break;
				}

			}

		}




	}
	int length = possibleIndices.size();
	if (length-2 > -1)
	{
		std::cout << "changing block with type" << blocks[possibleIndices[possibleIndices.size() - 2]].blockType << " to type 1 ";
		blocks[possibleIndices[possibleIndices.size() - 2]].blockType = 1;
	}
		
		
	
	
}



void Chunk::removeTargetBlock(glm::vec3 cameraPos,glm::vec3 cameraFront)
{
	int foundIndice{-1};
	bool found = false;
	float offset = 0.1;
	glm::vec3 nextCheckPos{};
	for (int y = 0; y < 1000; y++)
	{
	if(found)
	{
		break;
	}
	offset = offset + 0.1f;
	nextCheckPos = cameraPos + cameraFront * offset;
		for (int i = 0; i < blocks.size(); i++)
		{
			
			
			
			if (
				(blocks[i].cubeCenter.x - 0.5f < nextCheckPos.x && blocks[i].cubeCenter.x + 0.5f > nextCheckPos.x) &&
				(blocks[i].cubeCenter.y - 0.5f < nextCheckPos.y && blocks[i].cubeCenter.y + 0.5f > nextCheckPos.y) &&
				(blocks[i].cubeCenter.z - 0.5f < nextCheckPos.z && blocks[i].cubeCenter.z + 0.5f > nextCheckPos.z) 
				)
			{
				
				if (blocks[i].blockType == 1)
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
		blocks[foundIndice].blockType = 0;
	}
	
	
	
	
}


void Chunk::generateVerticesOfChunk()
{
	vertices.clear();
	int x, y, z;
	for (int i = 0; i < blocks.size(); i++)
	{
		x = i % 16;
		z = (i / 16) % 16;
		y = i / 256;

		blocks[i].cubeCenter.x = (static_cast<float>(x) + static_cast<float>(x) + 1.0f) / 2.0f;
		blocks[i].cubeCenter.y = (static_cast<float>(y) + static_cast<float>(y) + 1.0f) / 2.0f;
		blocks[i].cubeCenter.z = (static_cast<float>(z) + static_cast<float>(z) + 1.0f) / 2.0f;
			
		if (blocks[i].blockType == 0)
		{
			continue;
		}

		bool left = false;
		bool right = false;
		bool above = false;
		bool under = false;
		bool infront  = false;
		bool behind = false;
		
		if (y < 127)
		{
			if (blocks[x + (y+1)*16 * 16 + z*16].blockType == 1)
			{
				above = true;
			}
		}
		if (y > 0)
		{
			if (blocks[x + (y-1) * 16 * 16 + z * 16].blockType == 1)
			{
				under = true;
			}
		}
		if (x < 15)
		{
			if (blocks[x + 1 + y * 16 * 16 + z * 16].blockType == 1)
			{
				right = true;
			}
		}
		if (x > 0)
		{
			if (blocks[x - 1 + y * 16 * 16 + z * 16].blockType == 1)
			{
				left = true;
			}
		}
		if (z < 15)
		{
			if (blocks[x + y * 16 * 16 + (z+1) * 16].blockType == 1)
			{
				behind = true;
			}
		}
		if (z > 0)
		{
			if (blocks[x + y * 16 * 16 + (z-1) * 16].blockType == 1)
			{
				infront = true;
			}
		}

		if (left == true && right == true && above == true && under == true && infront == true && behind == true)
		{
			continue;

		}
		


		// przod (z = 0)
		vertices.push_back(0.0f + x); vertices.push_back(0.0f + y); vertices.push_back(0.0f + z); vertices.push_back(0.0f); vertices.push_back(0.0f);
		vertices.push_back(1.0f + x); vertices.push_back(0.0f + y); vertices.push_back(0.0f + z); vertices.push_back(1.0f); vertices.push_back(0.0f);
		vertices.push_back(1.0f + x); vertices.push_back(1.0f + y); vertices.push_back(0.0f + z); vertices.push_back(1.0f); vertices.push_back(1.0f);
		vertices.push_back(1.0f + x); vertices.push_back(1.0f + y); vertices.push_back(0.0f + z); vertices.push_back(1.0f); vertices.push_back(1.0f);
		vertices.push_back(0.0f + x); vertices.push_back(1.0f + y); vertices.push_back(0.0f + z); vertices.push_back(0.0f); vertices.push_back(1.0f);
		vertices.push_back(0.0f + x); vertices.push_back(0.0f + y); vertices.push_back(0.0f + z); vertices.push_back(0.0f); vertices.push_back(0.0f);

		// tyl (z = 1)
		vertices.push_back(0.0f + x); vertices.push_back(0.0f + y); vertices.push_back(1.0f + z); vertices.push_back(0.0f); vertices.push_back(0.0f);
		vertices.push_back(1.0f + x); vertices.push_back(0.0f + y); vertices.push_back(1.0f + z); vertices.push_back(1.0f); vertices.push_back(0.0f);
		vertices.push_back(1.0f + x); vertices.push_back(1.0f + y); vertices.push_back(1.0f + z); vertices.push_back(1.0f); vertices.push_back(1.0f);
		vertices.push_back(1.0f + x); vertices.push_back(1.0f + y); vertices.push_back(1.0f + z); vertices.push_back(1.0f); vertices.push_back(1.0f);
		vertices.push_back(0.0f + x); vertices.push_back(1.0f + y); vertices.push_back(1.0f + z); vertices.push_back(0.0f); vertices.push_back(1.0f);
		vertices.push_back(0.0f + x); vertices.push_back(0.0f + y); vertices.push_back(1.0f + z); vertices.push_back(0.0f); vertices.push_back(0.0f);

		// lewo (i = 0)
		vertices.push_back(0.0f + x); vertices.push_back(1.0f + y); vertices.push_back(1.0f + z); vertices.push_back(1.0f); vertices.push_back(0.0f);
		vertices.push_back(0.0f + x); vertices.push_back(1.0f + y); vertices.push_back(0.0f + z); vertices.push_back(1.0f); vertices.push_back(1.0f);
		vertices.push_back(0.0f + x); vertices.push_back(0.0f + y); vertices.push_back(0.0f + z); vertices.push_back(0.0f); vertices.push_back(1.0f);
		vertices.push_back(0.0f + x); vertices.push_back(0.0f + y); vertices.push_back(0.0f + z); vertices.push_back(0.0f); vertices.push_back(1.0f);
		vertices.push_back(0.0f + x); vertices.push_back(0.0f + y); vertices.push_back(1.0f + z); vertices.push_back(0.0f); vertices.push_back(0.0f);
		vertices.push_back(0.0f + x); vertices.push_back(1.0f + y); vertices.push_back(1.0f + z); vertices.push_back(1.0f); vertices.push_back(0.0f);

		// prawo (i = 1)
		vertices.push_back(1.0f + x); vertices.push_back(1.0f + y); vertices.push_back(1.0f + z); vertices.push_back(1.0f); vertices.push_back(0.0f);
		vertices.push_back(1.0f + x); vertices.push_back(1.0f + y); vertices.push_back(0.0f + z); vertices.push_back(1.0f); vertices.push_back(1.0f);
		vertices.push_back(1.0f + x); vertices.push_back(0.0f + y); vertices.push_back(0.0f + z); vertices.push_back(0.0f); vertices.push_back(1.0f);
		vertices.push_back(1.0f + x); vertices.push_back(0.0f + y); vertices.push_back(0.0f + z); vertices.push_back(0.0f); vertices.push_back(1.0f);
		vertices.push_back(1.0f + x); vertices.push_back(0.0f + y); vertices.push_back(1.0f + z); vertices.push_back(0.0f); vertices.push_back(0.0f);
		vertices.push_back(1.0f + x); vertices.push_back(1.0f + y); vertices.push_back(1.0f + z); vertices.push_back(1.0f); vertices.push_back(0.0f);

		// dol (y = 0)
		vertices.push_back(0.0f + x); vertices.push_back(0.0f + y); vertices.push_back(0.0f + z); vertices.push_back(0.0f); vertices.push_back(1.0f);
		vertices.push_back(1.0f + x); vertices.push_back(0.0f + y); vertices.push_back(0.0f + z); vertices.push_back(1.0f); vertices.push_back(1.0f);
		vertices.push_back(1.0f + x); vertices.push_back(0.0f + y); vertices.push_back(1.0f + z); vertices.push_back(1.0f); vertices.push_back(0.0f);
		vertices.push_back(1.0f + x); vertices.push_back(0.0f + y); vertices.push_back(1.0f + z); vertices.push_back(1.0f); vertices.push_back(0.0f);
		vertices.push_back(0.0f + x); vertices.push_back(0.0f + y); vertices.push_back(1.0f + z); vertices.push_back(0.0f); vertices.push_back(0.0f);
		vertices.push_back(0.0f + x); vertices.push_back(0.0f + y); vertices.push_back(0.0f + z); vertices.push_back(0.0f); vertices.push_back(1.0f);

		// gora (y = 1)
		vertices.push_back(0.0f + x); vertices.push_back(1.0f + y); vertices.push_back(0.0f + z); vertices.push_back(0.0f); vertices.push_back(1.0f);
		vertices.push_back(1.0f + x); vertices.push_back(1.0f + y); vertices.push_back(0.0f + z); vertices.push_back(1.0f); vertices.push_back(1.0f);
		vertices.push_back(1.0f + x); vertices.push_back(1.0f + y); vertices.push_back(1.0f + z); vertices.push_back(1.0f); vertices.push_back(0.0f);
		vertices.push_back(1.0f + x); vertices.push_back(1.0f + y); vertices.push_back(1.0f + z); vertices.push_back(1.0f); vertices.push_back(0.0f);
		vertices.push_back(0.0f + x); vertices.push_back(1.0f + y); vertices.push_back(1.0f + z); vertices.push_back(0.0f); vertices.push_back(0.0f);
		vertices.push_back(0.0f + x); vertices.push_back(1.0f + y); vertices.push_back(0.0f + z); vertices.push_back(0.0f); vertices.push_back(1.0f);


		

	}

}