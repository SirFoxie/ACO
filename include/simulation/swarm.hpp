#pragma once

#include "../simulation/world.hpp"
#include "../simulation/agent.hpp"


#define SWARM_SIZE 5

class Swarm {
public:
	static Swarm& getInstance() {
		static Swarm instance;
		return instance;
	}

	//Accessors
	int& size() { return swarmSize; }
	int& state() { return reinterpret_cast<int&>(SwarmState); }
	std::vector<Agent>& getAgents() { return agents; }

	void moveNearest(const Vector2& orgin, const Vector2& target);

	void update();
	void render();
private:
	Swarm(): swarmSize(SWARM_SIZE) { agents.resize(swarmSize); };
	~Swarm() = default;

	int swarmSize = 0;
	std::vector<Agent> agents;

	Agent::AgentState SwarmState = Agent::STANDBY;

	void debug();
};

void Swarm::moveNearest(const Vector2& orgin, const Vector2& target) {
	float min_distance = FLT_MAX;
	int id = 0, target_id = 0;
	for (Agent& agent : agents) {
		float curr_distance = Vector2Distance(orgin, agent.getPos());
		if (curr_distance < min_distance) {
			min_distance = curr_distance;
			target_id = id;
		}
		id++;
	}
	agents[target_id].addTargetPos(target);
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