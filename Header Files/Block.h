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
#include <map>

class Block
{
private:
	int blockType{ 0 };
	glm::vec3 cubeCenter{};
	std::map<int,std::pair<int,int>> tekstura;
public:
	Block() = default;
	Block(int type)
		: blockType{ type }
	{
		tekstura[1] = { 10,4 };
		tekstura[2] = { 10,5};
		tekstura[3] = { 0, 0};
		tekstura[4] = { 14,1 };

		//1 - dirt
		//2 - cobblestone
		//3 - lava
		//4 - diamond
	}
	friend std::ostream& operator<< (std::ostream& out, const Block& block)
	{
		out << "blokctype : " << block.blockType;
		return out;
	}
	std::pair<int,int> getPairFromMap(int index)
	{
		return tekstura[index];
	}

	friend class Chunk;
	friend class Camera;

};
