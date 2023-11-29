#pragma once

#include "../simulation/world.hpp"
#include "../simulation/agent.hpp"

class Swarm {
public:
	Swarm() = default;
	Swarm(int _size, World& _map);
	~Swarm() = default;

	//Accessors
	int& size() { return swarmSize; }

	void update();
	void render();
private:
	World& map;

	int swarmSize;
	std::vector<Agent> agents;

	void debug();
};

Swarm::Swarm(int _size, World& _map) :
	swarmSize(_size), agents(_size), map(_map)
{
	return;
}

void Swarm::update() {
	if (swarmSize != agents.size()) {
		agents.resize(swarmSize);
	}

	for (Agent& agent : agents) {
		agent.update(agents, {0.0, 0.0, 0.0});
	}
	return;
}

void Swarm::render() {
	for (Agent& agent : agents) {
		agent.render();
	}
	return;
}

void Swarm::debug() {
	DrawText(TextFormat("SWARM SIZE: %d", this->agents.size()), 0, 20, 10, BLUE);
}