#pragma once

#include "../simulation/agent.hpp"
#include "../simulation/rrt.hpp"

#define SWARM_SIZE 1

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

	void moveNearestAgent(const Vector2& target);

	void update();
	void render();
	void reset();
private:
	Swarm();
	~Swarm() = default;

	int swarmSize = 0;
	std::vector<Agent> agents;

	Agent::AgentState SwarmState = Agent::STANDBY;

	void debug();
};

Swarm::Swarm() : swarmSize(SWARM_SIZE) {
	agents.resize(swarmSize);

}

void Swarm::reset() {
	agents.clear();
	agents.resize(swarmSize);
}

void Swarm::moveNearestAgent(const Vector2& target) {
	float min_distance = FLT_MAX;
	int id = 0, target_id = 0;
	for (Agent& agent : agents) {
		float curr_distance = Vector2Distance(target, agent.getPos());
		if (curr_distance < min_distance) {
			min_distance = curr_distance;
			target_id = id;
		}
		id++;
	}
	//agents[target_id].addTargetPos(target);
	return;
}

void Swarm::update() {
	if (swarmSize != agents.size()) {
		agents.resize(swarmSize);
	}

	for (Agent& agent : agents) {
		if (!World::getInstance().allGoalsFound()) agent.state = Agent::EXPLORING;
		else agent.state = Agent::STANDBY;
		agent.update(agents, {0.0, 0.0, 0.0});
	}

	return;
}

void Swarm::render() {
	for (Agent& agent : agents) {
		agent.render();
	}
	//debug();
	return;
}

void Swarm::debug() {
	DrawText(TextFormat("SWARM SIZE: %d", this->agents.size()), 0, 20, 10, BLUE);
}