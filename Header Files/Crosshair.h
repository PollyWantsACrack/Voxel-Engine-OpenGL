#pragma once
#include <vector>

class CrossHair
{
private:
	std::vector<float> tablica{
	
	 0.15f/5.0f,  0.025f / 5.0f , 0.0f,  // top right
	 0.15f / 5.0f , -0.025f / 5.0f, 0.0f,  // bottom right
	-0.15f / 5.0f ,  0.025f / 5.0f, 0.0f,  // top left 
	
	 0.15f / 5.0f , -0.025f / 5.0f, 0.0f,  // bottom right
	-0.15f / 5.0f , -0.025f / 5.0f, 0.0f,  // bottom left
	-0.15f / 5.0f ,  0.025f / 5.0f , 0.0f,

	 0.015f / 5.0f,  0.25f / 5.0f, 0.0f,  // top right
	 0.015f / 5.0f, -0.25f / 5.0f, 0.0f,  // bottom right
	-0.015f / 5.0f,  0.25f / 5.0f, 0.0f,  // top left 
	
	 0.015f / 5.0f, -0.25f / 5.0f, 0.0f,  // bottom right
	-0.015f / 5.0f, -0.25f / 5.0f, 0.0f,  // bottom left
	-0.015f / 5.0f,  0.25f / 5.0f, 0.0f   // top left
	};
public:
	CrossHair()
	{
	}
	std::vector<float>& getVertices();
};


std::vector<float>& CrossHair::getVertices()
{
	return tablica;
}

