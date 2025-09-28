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


class Block
{
private:
	int blockType{ 0 };
	glm::vec3 cubeCenter{};
public:
	Block() = default;
	Block(int type)
		: blockType{ type }
	{

	}
	friend std::ostream& operator<< (std::ostream& out, const Block& block)
	{
		out << "blokctype : " << block.blockType;
		return out;
	}
	friend class Chunk;
	friend class Camera;

};
