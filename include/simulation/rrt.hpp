#pragma once

#include <vector>

#include "raylib.h"
#include "raymath.h"

#include "swarm.hpp"

struct Node
{
	Vector2 pos;
	int parent;
};

class rrt {
public:
	rrt() = delete;
	rrt(int _x, int _y, int _width, int _height);
	~rrt() = default;

	void addNode(Vector2 pos);

	Node getRandomNode();
	int getNearestNodeIdx(const Node& node);

	void update();
	void render();
private:
	std::vector<Node> nodes;
	int x, y, width, height;
	float stepSize = 100;
	Node randyNodey;
	Node latestNodey;

	bool isValidNode(const Vector2& node) const;
	void clampNode(Vector2& node);
};

rrt::rrt(int _x, int _y, int _width, int _height) :
	x(_x), y(_y), width(_width), height(_height)
{
	nodes.push_back(getRandomNode());
	for (auto& agent : Swarm::getInstance().getAgents()) {
		//nodes.push_back(agent.getPos());
	}
}

void rrt::addNode(Vector2 pos) {
	//nodes.push_back(pos);
	return;
}

Node rrt::getRandomNode() {
	Node randomNode;
	randomNode.pos = {	(float)GetRandomValue(x, width), 
						(float)GetRandomValue(y, height) };
	return randomNode;
}


int rrt::getNearestNodeIdx(const Node& node) {
	int nearestNodeIdx = -1;

	float min_distance = FLT_MAX;
	for (int i = 0; i < nodes.size(); ++i) {
		float curr_distance = Vector2Distance(nodes[i].pos, node.pos);
		if ( curr_distance < min_distance) {
			min_distance = curr_distance;
			nearestNodeIdx = i;
		}
	}
	return nearestNodeIdx;
}

void rrt::clampNode(Vector2& node) {
	if (node.x < this->x) node.x = this->x + 1;
	if (node.x > this->width) node.x = this->width - 1;
	if (node.y < this->y) node.y = this->y + 1;
	if (node.y > this->height) node.y = this->height - 1;
	return;
}

void rrt::update() {
	// spawn randNode in search area
	Node randNode = getRandomNode();
	randyNodey = randNode;

	// Find the nearest node to randNode
	int nearestNodeIdx = getNearestNodeIdx(randNode);
	Node nearestNode = nodes[nearestNodeIdx];

	float theta = Vector2Angle(nearestNode.pos, randNode.pos);
	Node newNode;
	newNode.pos= { nearestNode.pos.x + (stepSize * cos(theta)), 
					nearestNode.pos.y + (stepSize * sin(theta)) };
	newNode.parent = nearestNodeIdx;
	
	//clampNode(newNode);
	nodes.push_back(newNode);
	latestNodey = newNode;
	//Swarm::getInstance().moveNearest(nearNode, newNode);	
}

void rrt::render() {
	DrawText(TextFormat("Nodes: %d", nodes.size()), GetMouseX(), GetMouseY(), 10, LIME);

	// Draw edges
	for (int i = 1; i < nodes.size(); ++i) {
		DrawLineEx(nodes[i].pos, nodes[nodes[i].parent].pos, 3,  GRAY);
	}

	for (const auto& Node : this->nodes) {
		DrawCircleV(Node.pos, 5, RED);
	}

	if (this->nodes.size() > 1){
		DrawCircleV(randyNodey.pos, 5, GREEN);
		DrawLineEx(latestNodey.pos, randyNodey.pos, 3, GREEN);
	}
	return;
}