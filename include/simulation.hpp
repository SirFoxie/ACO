
#include <vector>

#include "world.hpp"
#include "agent.hpp"

class Simulation {
private:
	World world;
	std::vector<Agent> agents;

public:
	Simulation(World _world, int AgentNum): world(_world)
	{
		agents.resize(AgentNum);
	}

	void addAgent() {

	}

	void removeAgent() {

	}

	std::vector<Agent> getAgentList() {
		return agents;
	}

};