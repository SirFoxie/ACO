#pragma once

#include <vector>

#include "raylib.h"
#include "raymath.h"

#include "swarm.hpp"

struct Node
{
	Vector2 pos;
	int parentIdx = -1;
	float costToOrgin = 0.0;
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
	float costToParent(int childIdx, int parentIdx);
	void rewireTree(int newIdx);
};

rrt::rrt(int _x, int _y, int _width, int _height) :
	x(_x), y(_y), width(_width), height(_height)
{
	nodes.push_back(getRandomNode());
	for (auto& agent : Swarm::getInstance().getAgents()) {
		//nodes.push_back({ agent.getPos() });
	}
}

void rrt::addNode(Vector2 pos) {
	Node newNode;
	newNode.pos = pos;
	newNode.parentIdx = getNearestNodeIdx(newNode);

	randyNodey = newNode;
	latestNodey = newNode;
	nodes.push_back(newNode);
	rewireTree(nodes.size() - 1);
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

float rrt::costToParent(int childIdx, int parentIdx) {
	return Vector2Distance(nodes[childIdx].pos, nodes[parentIdx].pos);
}

void rrt::rewireTree(int newIdx) {
	for (int i = 1; i < nodes.size(); ++i) {
		float newCost = nodes[newIdx].parentIdx == -1 ?
			0 : costToParent(newIdx, nodes[newIdx].parentIdx);
		float costThroughNewNode = newCost + costToParent(i, newIdx);
		if (costThroughNewNode < costToParent(i, nodes[i].parentIdx)) {
			nodes[newIdx].parentIdx = nodes[i].parentIdx;
			//nodes[i].parentIdx = newIdx;
		}
	}
}

void rrt::update() {
	// spawn randNode in search area
	Node randNode = getRandomNode();
	randyNodey = randNode;

	// Find the nearest node to randNode
	int nearestNodeIdx = getNearestNodeIdx(randNode);
	Node nearestNode = nodes[nearestNodeIdx];

	Node newNode;
	if (Vector2Distance(randNode.pos, nearestNode.pos) > stepSize) {
		float theta = Vector2Angle(nearestNode.pos, randNode.pos);
		newNode.pos = { nearestNode.pos.x + (stepSize * cos(theta)),
						nearestNode.pos.y + (stepSize * sin(theta)) };
	}
	else newNode.pos = randNode.pos;
	newNode.parentIdx = nearestNodeIdx;
	
	// Add Node to tree
	latestNodey = newNode;
	clampNode(newNode.pos);
	nodes.push_back(newNode);
	rewireTree(nodes.size()-1);

	Swarm::getInstance().moveNearest(nearestNode.pos, newNode.pos);
}

void rrt::render() {
	DrawText(TextFormat("Nodes: %d", nodes.size()), GetMouseX(), GetMouseY(), 10, LIME);

	// Draw edges
	for (int i = 1; i < nodes.size(); ++i) {
		DrawLineEx(nodes[i].pos, nodes[nodes[i].parentIdx].pos, 3,  GRAY);
	}

	int i = 0;
	for (const auto& Node : this->nodes) {
		DrawCircleV(Node.pos, 5, RED);
		DrawText(TextFormat("Idx: %d", i++), Node.pos.x, Node.pos.y, 10, LIME);
		DrawText(TextFormat("ParentIdx: %d", Node.parentIdx), Node.pos.x, Node.pos.y + 12, 10, LIME);
	}

	DrawCircleV(nodes[0].pos, 10, RED);

	if (this->nodes.size() > 1){
		DrawCircleV(randyNodey.pos, 5, GREEN);
		DrawLineEx(latestNodey.pos, randyNodey.pos, 3, GREEN);
	}
	return;
}