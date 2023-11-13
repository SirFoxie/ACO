#pragma once
#include <utility>

class Agent {
private:
	float		AgentTileX	= 0;
	float		AgentTileY	= 0;
	float		AgentMovSpd = 1;

	float		AgentSizeX = 10;
	float		AgentSizeY = 10;
public:
	//Ctor
	Agent() :AgentTileX(0), AgentTileY(0), AgentMovSpd(1) {}

	Agent(float x, float y, float spd) 
		:AgentTileX(x), AgentTileY(y), AgentMovSpd(spd) 
	{}
	// Move Agent in spd increments
	void Update(int IncX, int IncY) {
		AgentTileX += IncX * AgentMovSpd;
		AgentTileY += IncY * AgentMovSpd;
		return;
	}

	// Return Agent X Y in std::pair<int, int>
	std::pair<int,int> ReadAgentPos() {
		return std::make_pair(AgentTileX, AgentTileY);
	}

	float PosX() const{ return AgentTileX; }
	float PosY() const{ return AgentTileY; }
	float SizeX() const{ return AgentSizeX; }
	float SizeY() const{ return AgentSizeY; }
};