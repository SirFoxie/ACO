#pragma once

#include <queue>

#include "raylib.h"

#include "../common/common.hpp"
#include "../simulation/world.hpp"

#include "./simulation/rrt.hpp"

#define MIN_VEL -2
#define MAX_VEL 2

#define MAX_FORCE 0.1

#define AGENT_SIZE 5

class Agent {
public:
	enum AgentState { STANDBY, EXPLORING, SEEKING, RESCUE } state;

	struct Weight {
		float alignment;
		float cohesion;
		float seperation;
	};

	Agent();
	~Agent() = default;

	Vector2 getPos() { return position; }

	void addTargetPos(Vector2& _targetPos) { 
		targetPos = _targetPos; 
		state = SEEKING; 
	}

	void addTargetNode(const Node& target) { 
		//targetQueue.push_back(target); 
		//state = SEEKING; 
	}

	Vector2 seek(const Vector2& target);
	
	Node targetNode;
	Node currNode;
	Vector2 targetPos;

	void update(const std::vector <Agent>& agents, const Weight& agentsWeights);
	void render();

private:
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;

	int viewRadius = 150 + (AGENT_SIZE/2);

	rrt planner;
	int pathIdx = 0;
	std::vector<Node> path;


	bool Raycast(Vector2 origin, Vector2 direction, float Distance);

	Vector2 separation(const std::vector <Agent>& agents);
	Vector2 avoid();
	void edge();

	bool checkObsHit();
	void checkGoal();
	void updateMap();
	void debug();
};

Agent::Agent() {
	Area StartArea = World::getInstance().getStartArea();
	this->position = getRandomPixelPos(StartArea.x+2, StartArea.y+2, StartArea.width-2, StartArea.height-2);
	this->velocity = { (float)GetRandomValue(MIN_VEL, MAX_VEL), 
						(float)GetRandomValue(MIN_VEL, MAX_VEL) };
	this->acceleration = { 1.0, 1.0 };
	this->state = STANDBY;
	this->currNode = { position, 0 };
	this->targetNode = currNode;
	planner.addNode(position, 0);
	planner.update();
	return;
}

void Agent::update(const std::vector <Agent>& agents, const Weight& agentsWeights) {

	switch (state) {
	case STANDBY:
		return;
		break;
	case SEEKING:

		break;
	case EXPLORING:

		if (path.empty() || pathIdx >= path.size()) {
			planner.update();
			targetNode = planner.getTargetNode();
			path = planner.getPath(currNode, targetNode);
			pathIdx = 0;
		}

		else if(Vector2Distance(position, currNode.pos) < 1){
			currNode = path[pathIdx++];
		}

		Vector2 exploringForce = seek(currNode.pos);
		this->acceleration = Vector2Add(acceleration, Vector2Scale(exploringForce, 0.5));
		
		break;
	}

	Vector2 separationForce = separation(agents);
	this->acceleration = Vector2Add(acceleration, Vector2Scale(separationForce, 1.0));
	
	// avoid static obstacles
	//Vector2 avoidanceForce = avoid(); 
	//this->acceleration = Vector2Add(acceleration, Vector2Scale(avoidanceForce, 0.0));

	this->acceleration = Vector2Scale(acceleration, 4.0);
	this->velocity = Vector2Add(velocity, acceleration);
	this->velocity = Vector2ClampValue(velocity, MIN_VEL, MAX_VEL);
	this->position = Vector2Add(position, velocity);
	this->acceleration = Vector2Scale(acceleration, 0);

	edge();
	//if ( checkObsHit()) planner.removeNode(currNode);
	updateMap();
	return;
}

bool Agent::checkObsHit() {
	for (auto& obs : World::getInstance().getObs()) {
		if (CheckCollisionCircles(position, AGENT_SIZE, obs.pos, obs.size)) {
			return true;
		}
	}
	return false;
}

void Agent::checkGoal() {
	for (auto& goal : World::getInstance().getGoals()) {
		if (goal.found == false)
			goal.found = (CheckCollisionCircles(position, AGENT_SIZE, goal.pos, 20));
	}
	return;
}

void Agent::updateMap() {
	World& map = World::getInstance();
	Vector2 cell_pos = calcCellCoords(position);
	if (map.checkTileType(cell_pos.x, cell_pos.y) == World::Fog) {
		map.setTileType(cell_pos.x, cell_pos.y, World::Explored);
	}

	checkGoal();
}

Vector2 Agent::separation(const std::vector <Agent>& agents) {
	Vector2 steering = { 0.0, 0.0 };
	float perception = 5 * AGENT_SIZE;
	int totalSurroundingAgents = 0;

	for (const Agent& otherboid : agents) {
		if (&otherboid != this) {
			float distance = Vector2Distance(this->position, otherboid.position);
			if ((distance > 0) && (distance < perception)) {
				Vector2 diff = Vector2Subtract(this->position, otherboid.position);
				diff = Vector2Scale(diff, distance * distance);
				steering = Vector2Add(steering, diff);
				totalSurroundingAgents++;
			}
		}
	}
	if (totalSurroundingAgents > 0) {
		steering = Vector2Scale(steering, 1.0 / (float)totalSurroundingAgents);
		steering = Vector2Scale(steering, MAX_VEL);
		steering = Vector2Subtract(steering, this->velocity);
		steering = Vector2ClampValue(steering, -MAX_FORCE, MAX_FORCE);
	}
	return steering;
}

Vector2 Agent::seek(const Vector2& target) {

	Vector2 steering = { 0.0, 0.0 };
	Vector2 desired = Vector2Subtract(target, position);

	desired = Vector2Normalize(desired);
	desired = Vector2Scale(desired, MAX_VEL);

	steering = Vector2Subtract(desired, velocity);
	steering = Vector2ClampValue(steering, -MAX_FORCE, MAX_FORCE);
	return steering;
}


Vector2 Agent::avoid() {
	Area borders = GridAreaToPixelArea(World::getInstance().getMap());

	Vector2 steering = { 0.0, 0.0 };
	float perception = 10 * AGENT_SIZE;

	// Get Distance from Obstacle
	//bool obstacleInPath = Raycast(player.position, direction, 100.0f, obstacle);


	return steering;
}

void Agent::edge() {
	Area borders = GridAreaToPixelArea(World::getInstance().getMap());

	float perception = 1 * AGENT_SIZE;

	if (position.x < (borders.x + perception) || position.x > (borders.width - perception)) {
		this->velocity.x = -(this->velocity.x);
	}
	if (position.y < (borders.y + perception) || position.y >(borders.height - perception)) {
		this->velocity.y = -(this->velocity.y);
	}

}

void Agent::render() {

	planner.render();
	DrawCircle(position.x, position.y, AGENT_SIZE, ORANGE);
	debug();
	return;
}

void Agent::debug() {
	DrawCircleV(currNode.pos, AGENT_SIZE, PURPLE);
	DrawCircleV(targetNode.pos, AGENT_SIZE, PURPLE);
	DrawLineV(currNode.pos, targetNode.pos, PURPLE);

	DrawCircleLines(position.x, position.y, this->viewRadius, PINK);
	DrawText(TextFormat("[%.2f, %.2f]", position.x, position.y), position.x, position.y, 10, LIME);
	DrawText(TextFormat("[%.2f, %.2f]", velocity.x, velocity.y), position.x, position.y + 10, 10, LIME);
	DrawText(TextFormat("State: %d", this->state), position.x, position.y + 20, 10, LIME);


}