#pragma once

#include "raylib.h"

#include "../common/common.hpp"
#include "../simulation/world.hpp"

#define MIN_VEL -5
#define MAX_VEL 5

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
	void addTargetPos(const Vector2& target) { targetPos = target; state = SEEKING; }
	Vector2 seek(const Vector2& target);

	void update(const std::vector <Agent>& agents, const Weight& agentsWeights);
	void render();

private:

	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;

	Vector2 targetPos;

	bool Raycast(Vector2 origin, Vector2 direction, float Distance);

	Vector2 separation(const std::vector <Agent>& agents);
	Vector2 avoid();
	void edge();

	void updateMap();
	void debug();
};

Agent::Agent() {
	World::Area StartArea = GridAreaToPixelArea((World::getInstance().getStartArea()));
	this->position = { (float)GetRandomValue(StartArea.x + (AGENT_SIZE * 2) , StartArea.width -(AGENT_SIZE * 2)),
						(float)GetRandomValue(StartArea.y + (AGENT_SIZE * 2), StartArea.height -(AGENT_SIZE * 2)) };
	this->velocity = { (float)GetRandomValue(MIN_VEL, MAX_VEL), 
						(float)GetRandomValue(MIN_VEL, MAX_VEL) };
	this->acceleration = { 1.0, 1.0 };
	this->state = STANDBY;
	return;
}

void Agent::update(const std::vector <Agent>& agents, const Weight& agentsWeights) {

	switch (state) {
	case STANDBY:
		return;
		break;
	case SEEKING:
		Vector2 seekingForce = seek(targetPos);
		this->acceleration = Vector2Add(acceleration, Vector2Scale(seekingForce, 10));
		break;
	case EXPLORING:

		break;
	}

	Vector2 separationForce = separation(agents);
	this->acceleration = Vector2Add(acceleration, Vector2Scale(separationForce, 10.0));
	
	Vector2 avoidanceForce = avoid(); // avoid static obstacles
	this->acceleration = Vector2Add(acceleration, Vector2Scale(avoidanceForce, 10.0));

	this->acceleration = Vector2Scale(acceleration, 0.4);
	this->velocity = Vector2Add(velocity, acceleration);
	this->velocity = Vector2ClampValue(velocity, MIN_VEL, MAX_VEL);
	this->position = Vector2Add(position, velocity);
	this->acceleration = Vector2Scale(acceleration, 0);

	edge();
	updateMap();
	return;
}

void Agent::updateMap() {
	World& map = World::getInstance();
	Vector2 cell_pos = calcCellCoords(position);
	if (map.checkTileType(cell_pos.x, cell_pos.y) == World::Fog) {
		map.setTileType(cell_pos.x, cell_pos.y, World::Explored);
	}
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
		steering = Vector2ClampValue(steering, -0.2, 0.2);
	}
	return steering;
}

Vector2 Agent::seek(const Vector2& target) {
	if (Vector2Equals(position, target)) state = STANDBY;

	Vector2 steering = { 0.0, 0.0 };
	Vector2 distance = Vector2Subtract(target, position);


	steering = Vector2Scale(steering, MAX_VEL);
	steering = Vector2Subtract(distance, velocity);
	steering = Vector2ClampValue(steering, -0.1, 0.1);

	return steering;
}

//bool Raycast();

Vector2 Agent::avoid() {
	World::Area borders = GridAreaToPixelArea(World::getInstance().getMap());

	Vector2 steering = { 0.0, 0.0 };
	float perception = 10 * AGENT_SIZE;

	// Get Distance from Obstacle
	//bool obstacleInPath = Raycast(player.position, direction, 100.0f, obstacle);


	return steering;
}

void Agent::edge() {
	World::Area borders = GridAreaToPixelArea(World::getInstance().getMap());

	float perception = 1 * AGENT_SIZE;

	if (position.x < (borders.x + perception) || position.x > (borders.width - perception)) {
		this->velocity.x = -(this->velocity.x);
	}
	if (position.y < (borders.y + perception) || position.y >(borders.height - perception)) {
		this->velocity.y = -(this->velocity.y);
	}

}

void Agent::render() {
	DrawCircle(position.x, position.y, AGENT_SIZE, ORANGE);
	debug();
	return;
}

void Agent::debug() {
	DrawText(TextFormat("[%.2f, %.2f]", position.x, position.y), position.x, position.y, 10, LIME);
	DrawText(TextFormat("[%.2f, %.2f]", velocity.x, velocity.y), position.x, position.y + 10, 10, LIME);
	DrawText(TextFormat("State: %d", this->state), position.x, position.y + 20, 10, LIME);
	DrawText(TextFormat("Target: [%.2f, %.2f]", this->targetPos.x, this->targetPos.y), position.x, position.y + 30, 10, LIME);
}