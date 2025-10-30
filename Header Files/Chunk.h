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
#include <map>


class Chunk 
{
private:
	
	std::vector<Block> blocks;
	int clickedBlockIndex = 0;
	int offset = 1;
	

public:
		
	std::vector<float> vertices;
	int getChunkSize()
	{
		return blocks.size();
	}
	glm::vec3 getBlockCenter(int i)
	{
		return blocks[i].cubeCenter;
	}
	int getBlockType(int index)
	{
		return blocks.at(index).blockType;
	}
	void setBlockType(int indice,int type)
	{
		blocks[indice].blockType = type;
	}
	void setBlockType(std::vector<int> possibleIndices,int type)
	{
		blocks[possibleIndices[possibleIndices.size() - 2]].blockType = type;
		clickedBlockIndex = possibleIndices[possibleIndices.size() - 2];
	}
	void setclickedBlockIndex(int index)
	{
		clickedBlockIndex = index;
	}
	
	
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
	
	
	
};

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
		float u = 0.0333333333333333333333333333333333333333333333333f;
		float v = 0.0588235294117647f;
		float nx = blocks[i].getPairFromMap(blocks[i].blockType).first;
		float ny = blocks[i].getPairFromMap(blocks[i].blockType).second;
		float gutter = 0.01f;

		
		
		
		
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
		vertices.push_back(0.0f + x); vertices.push_back(0.0f + y); vertices.push_back(0.0f + z); vertices.push_back(u * nx); vertices.push_back(v * ny);
		vertices.push_back(1.0f + x); vertices.push_back(0.0f + y); vertices.push_back(0.0f + z); vertices.push_back(u * nx + u); vertices.push_back(v * ny);
		vertices.push_back(1.0f + x); vertices.push_back(1.0f + y); vertices.push_back(0.0f + z); vertices.push_back(u * nx + u); vertices.push_back(v * ny + v);
		vertices.push_back(1.0f + x); vertices.push_back(1.0f + y); vertices.push_back(0.0f + z); vertices.push_back(u * nx + u); vertices.push_back(v * ny + v);
		vertices.push_back(0.0f + x); vertices.push_back(1.0f + y); vertices.push_back(0.0f + z); vertices.push_back(u * nx); vertices.push_back(v * ny + v);
		vertices.push_back(0.0f + x); vertices.push_back(0.0f + y); vertices.push_back(0.0f + z); vertices.push_back(u * nx); vertices.push_back(v * ny);

		// tyl (z = 1)
		vertices.push_back(0.0f + x); vertices.push_back(0.0f + y); vertices.push_back(1.0f + z); vertices.push_back(u * nx); vertices.push_back(v * ny);
		vertices.push_back(1.0f + x); vertices.push_back(0.0f + y); vertices.push_back(1.0f + z); vertices.push_back(u * nx + u); vertices.push_back(v * ny);
		vertices.push_back(1.0f + x); vertices.push_back(1.0f + y); vertices.push_back(1.0f + z); vertices.push_back(u * nx + u); vertices.push_back(v * ny + v);
		vertices.push_back(1.0f + x); vertices.push_back(1.0f + y); vertices.push_back(1.0f + z); vertices.push_back(u * nx + u); vertices.push_back(v * ny + v);
		vertices.push_back(0.0f + x); vertices.push_back(1.0f + y); vertices.push_back(1.0f + z); vertices.push_back(u * nx); vertices.push_back(v * ny + v);
		vertices.push_back(0.0f + x); vertices.push_back(0.0f + y); vertices.push_back(1.0f + z); vertices.push_back(u * nx); vertices.push_back(v * ny);

		// lewo (i = 0)
		vertices.push_back(0.0f + x); vertices.push_back(1.0f + y); vertices.push_back(1.0f + z); vertices.push_back(u * nx + u); vertices.push_back(v * ny);
		vertices.push_back(0.0f + x); vertices.push_back(1.0f + y); vertices.push_back(0.0f + z); vertices.push_back(u * nx + u); vertices.push_back(v * ny + v);
		vertices.push_back(0.0f + x); vertices.push_back(0.0f + y); vertices.push_back(0.0f + z); vertices.push_back(u * nx); vertices.push_back(v * ny + v);
		vertices.push_back(0.0f + x); vertices.push_back(0.0f + y); vertices.push_back(0.0f + z); vertices.push_back(u * nx); vertices.push_back(v * ny + v);
		vertices.push_back(0.0f + x); vertices.push_back(0.0f + y); vertices.push_back(1.0f + z); vertices.push_back(u * nx); vertices.push_back(v * ny);
		vertices.push_back(0.0f + x); vertices.push_back(1.0f + y); vertices.push_back(1.0f + z); vertices.push_back(u * nx + u); vertices.push_back(v * ny);

		// prawo (i = 1)
		vertices.push_back(1.0f + x); vertices.push_back(1.0f + y); vertices.push_back(1.0f + z); vertices.push_back(u * nx + u); vertices.push_back(v * ny);
		vertices.push_back(1.0f + x); vertices.push_back(1.0f + y); vertices.push_back(0.0f + z); vertices.push_back(u * nx + u); vertices.push_back(v * ny + v);
		vertices.push_back(1.0f + x); vertices.push_back(0.0f + y); vertices.push_back(0.0f + z); vertices.push_back(u * nx); vertices.push_back(v * ny + v);
		vertices.push_back(1.0f + x); vertices.push_back(0.0f + y); vertices.push_back(0.0f + z); vertices.push_back(u * nx); vertices.push_back(v * ny + v);
		vertices.push_back(1.0f + x); vertices.push_back(0.0f + y); vertices.push_back(1.0f + z); vertices.push_back(u * nx); vertices.push_back(v * ny);
		vertices.push_back(1.0f + x); vertices.push_back(1.0f + y); vertices.push_back(1.0f + z); vertices.push_back(u * nx + u); vertices.push_back(v * ny);

		// dol (y = 0)
		vertices.push_back(0.0f + x); vertices.push_back(0.0f + y); vertices.push_back(0.0f + z); vertices.push_back(u * nx); vertices.push_back(v * ny + v);
		vertices.push_back(1.0f + x); vertices.push_back(0.0f + y); vertices.push_back(0.0f + z); vertices.push_back(u * nx + u); vertices.push_back(v * ny + v);
		vertices.push_back(1.0f + x); vertices.push_back(0.0f + y); vertices.push_back(1.0f + z); vertices.push_back(u * nx + u); vertices.push_back(v * ny);
		vertices.push_back(1.0f + x); vertices.push_back(0.0f + y); vertices.push_back(1.0f + z); vertices.push_back(u * nx + u); vertices.push_back(v * ny);
		vertices.push_back(0.0f + x); vertices.push_back(0.0f + y); vertices.push_back(1.0f + z); vertices.push_back(u * nx); vertices.push_back(v * ny);
		vertices.push_back(0.0f + x); vertices.push_back(0.0f + y); vertices.push_back(0.0f + z); vertices.push_back(u * nx); vertices.push_back(v * ny + v);

		// gora (y = 1)
		vertices.push_back(0.0f + x); vertices.push_back(1.0f + y); vertices.push_back(0.0f + z); vertices.push_back(u * nx); vertices.push_back(v * ny + v);
		vertices.push_back(1.0f + x); vertices.push_back(1.0f + y); vertices.push_back(0.0f + z); vertices.push_back(u * nx + u); vertices.push_back(v * ny + v);
		vertices.push_back(1.0f + x); vertices.push_back(1.0f + y); vertices.push_back(1.0f + z); vertices.push_back(u * nx + u); vertices.push_back(v * ny);
		vertices.push_back(1.0f + x); vertices.push_back(1.0f + y); vertices.push_back(1.0f + z); vertices.push_back(u * nx + u); vertices.push_back(v * ny);
		vertices.push_back(0.0f + x); vertices.push_back(1.0f + y); vertices.push_back(1.0f + z); vertices.push_back(u * nx); vertices.push_back(v * ny);
		vertices.push_back(0.0f + x); vertices.push_back(1.0f + y); vertices.push_back(0.0f + z); vertices.push_back(u * nx); vertices.push_back(v * ny + v);


		

	}

}