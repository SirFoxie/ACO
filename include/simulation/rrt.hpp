#pragma once

#include <vector>

#include "raylib.h"
#include "raymath.h"


struct Node
{
	Vector2 pos;
	int idx = 0;
	int parentIdx = -1;

	bool operator== (Node& other) {
		return (((this->pos.x - other.pos.x) < 0.001) 
			&& ((this->pos.y - other.pos.y) < 0.001) 
			&& (this->idx == other.idx));
	}

	bool operator== (const Node& other) const{
		return (((this->pos.x - other.pos.x) < 0.001)
			&& ((this->pos.y - other.pos.y) < 0.001)
			&& (this->idx == other.idx));
	}

	bool operator!= (Node& other) {
		return !(this == &other);
	}
};

class rrt {
public:
	rrt();
	~rrt() = default;

	void setArea(int _x, int _y, int _width, int _height);
	void addNode(Vector2 pos, int nodeIdx = -1);
	void removeNode(Node nodeToRemove);
	void addGoal(Vector2 pos);
	void addObs	(Vector2 pos);

	bool allGoalFound();

	Node& getTargetNode() { return latestNodey; }
	std::vector<Node> getPath(Node& orgin, Node& target);
	Node growTree();

	void update();
	void render();
	void reset();

private:
	//std::vector<Goal> goals;
	//std::vector<Node> goalPaths;
	//int endGoalNodeIdx = -1;

	Area SearchArea;
	float stepSize = 50;

	Node randyNodey;
	Node latestNodey;
	Node targetNodey;
	std::vector<Node> currPath;

	std::vector<Node> nodes;

	// Helper
	void clampNode(Vector2& node);

	// Basic RRT
	Node& getRandomNode();
	Node& getNearestNode(std::vector<Node>& nodes, const Node& node);
	bool isValidNode(const Vector2& node) const;

	// RRT*
	std::vector <int> getNeighbourNodesIdx(const Node& node);
	float costToParent(int childIdx, int parentIdx);
	void rewireTree(int newidx);

	int rrt::FindCommonAncestor(int node1Index, int node2Index);
	void rrt::connectTree(Node newNode);
};

rrt::rrt() { 
	SearchArea = World::getInstance().getSearchArea();
	reset(); 
}

void rrt::reset() {
	// Use std::remove_if to find elements with the specified value

	auto removeCondition = [](const Node& node) {
		return node.parentIdx > -1;
		};

	auto newEnd = std::remove_if(nodes.begin(), nodes.end(), removeCondition);

	// Erase the elements with the specified value
	nodes.erase(newEnd, nodes.end());

	randyNodey.pos = { 0,0 };
	latestNodey.pos = { 0,0 };
	targetNodey = getRandomNode();
}

void rrt::setArea(int _x, int _y, int _width, int _height) {
	SearchArea.x = _x ; 
	SearchArea.y = _y ;
	SearchArea.width = _width ;
	SearchArea.height = _height ;
}

void rrt::addObs(Vector2 pos) {

	return;
}

void rrt::addNode(Vector2 pos, int nodeIdx){
	Node newNode = { pos, nodeIdx };
	nodes.push_back(newNode);
}

void rrt::removeNode(Node nodeToRemove) {
	auto it = std::find(nodes.begin(), nodes.end(), nodeToRemove);
	if (it != nodes.end()) {
		nodes.erase(it);
	}
}

Node& rrt::getRandomNode() {
	Node randomNode = { World::getInstance().getRandomFogPoint() };
	randyNodey = randomNode;
	return randomNode;
}

Node& rrt::getNearestNode(std::vector<Node>& nodes, const Node& node) {
	int nearestNodeIdx = -1;

	float min_distance = FLT_MAX;
	for (int i = 0; i < nodes.size(); ++i) {
		float curr_distance = Vector2Distance(nodes[i].pos, node.pos);
		if ( curr_distance < min_distance) {
			min_distance = curr_distance;
			nearestNodeIdx = i;
		}
	}
	return nodes[nearestNodeIdx];
}

std::vector<int> rrt::getNeighbourNodesIdx(const Node& currNode) {
	std::vector<int> neighbourNodesIdx;
	for (auto& node : nodes) {
		if (Vector2Distance(node.pos, currNode.pos) <= stepSize) {
			neighbourNodesIdx.push_back(node.idx);
		}
	}

	return neighbourNodesIdx;
}

void rrt::clampNode(Vector2& node) {
	if (node.x < this->SearchArea.x * CELL_SIZE) node.x = this->SearchArea.x * CELL_SIZE + 1;
	if (node.x > (this->SearchArea.width + this->SearchArea.x) * CELL_SIZE) node.x = (this->SearchArea.width + this->SearchArea.x) * CELL_SIZE - 1;
	if (node.y < this->SearchArea.y * CELL_SIZE) node.y = this->SearchArea.y * CELL_SIZE + 1;
	if (node.y > (this->SearchArea.height + this->SearchArea.y) * CELL_SIZE) node.y = (this->SearchArea.height + this->SearchArea.y) * CELL_SIZE - 1;
	return;
}

int rrt::FindCommonAncestor(int node1Index, int node2Index) {
	std::vector<bool> visited(nodes.size(), false);

	// Traverse from node1 to the root and mark visited nodes
	while (node1Index != -1) {
		visited[node1Index] = true;
		node1Index = nodes[node1Index].parentIdx;
	}

	// Traverse from node2 to the root and find the common ancestor
	while (node2Index != -1) {
		if (visited[node2Index]) {
			// Common ancestor found
			return node2Index;
		}
		node2Index = nodes[node2Index].parentIdx;
	}

	// No common ancestor found
	return -1;
}

std::vector<Node> rrt::getPath(Node& orgin, Node& target) {
	
	// Find the common parent
	int commonAncestor = FindCommonAncestor(orgin.idx, target.idx);
	int currentIdx;

	currPath.clear();

	// Trace orgin -> common parent
	std::vector<Node> orginPath;
	currentIdx = orgin.idx;
	while (currentIdx != commonAncestor) {
		orginPath.push_back(nodes[currentIdx]);
		currentIdx = nodes[currentIdx].parentIdx;
	}

	// Trace target -> common parent
	std::vector<Node> targetPath;
	currentIdx = target.idx;
	while (currentIdx >= commonAncestor && currentIdx != -1) {
		targetPath.push_back(nodes[currentIdx]);
		currentIdx = nodes[currentIdx].parentIdx;
	}

	targetPath.back().parentIdx = -1;

	std::reverse(targetPath.begin(), targetPath.end());

	currPath = orginPath;
	currPath.insert(currPath.end(), targetPath.begin(), targetPath.end());

	return currPath;
}

Node rrt::growTree() {

	Node newNode;
	Node randNode = getRandomNode();
	Node nearNode = getNearestNode(nodes, randNode);

	float distance = std::min(Vector2Distance(randNode.pos, nearNode.pos), stepSize);
	float theta = Vector2Angle(nearNode.pos, randNode.pos);

	newNode.pos = { nearNode.pos.x + (distance * cos(theta)),
					nearNode.pos.y + (distance * sin(theta)) };

	newNode.idx = nodes.size();
	newNode.parentIdx = nearNode.idx;

	// Add Node to tree
	latestNodey = newNode;
	//clampNode(newNode.pos);
	nodes.push_back(newNode);

	return newNode;
}

void rrt::update() {

	growTree();
	//updatePath(nodes[nodes.size()-2], nodes.back());

}

void rrt::render() {
	Vector2 mouseRel = GlobalData::getInstance().getMouseRel();
	//DrawText(TextFormat("Trees: %d", trees.size()), mouseRel.x, mouseRel.y + 10, 10, LIME);
	DrawText(TextFormat("Nodes: %d", nodes.size()), mouseRel.x, mouseRel.y+20, 10, LIME);
	DrawText(TextFormat("Path: %d", currPath.size()), mouseRel.x, mouseRel.y + 30, 10, LIME);


	// Draw Tree	
	for (auto& node: this->nodes){
		// Nodes
		DrawCircleV(node.pos, (node.parentIdx == -1) ? 10 : 5, (node.parentIdx == -1) ? RED : Fade(RED, 0.5));
		// Paths
		if (node.parentIdx != -1) {
			DrawLineEx(node.pos, nodes[node.parentIdx].pos, 3, Fade(GRAY, 0.5));
		}
		//DrawText(TextFormat("Idx: %d", node.idx), node.pos.x, node.pos.y, 10, LIME);
		//DrawText(TextFormat("ParentIdx: %d", node.parentIdx), node.pos.x, node.pos.y + 12, 10, LIME);
	}

	// Draw edges
	int i = 0;
	for (auto& node : this->currPath) {
		DrawCircleV(node.pos, 3, PINK);
		//DrawText(TextFormat("Idx: %d", i++), node.pos.x, node.pos.y, 10, LIME);
		if (node.parentIdx != -1) {
			DrawLineEx(node.pos, nodes[node.parentIdx].pos, 3, Fade(PINK,0.8));
		}
	}

	// Draw decisions
	DrawCircleV(randyNodey.pos, 5, PURPLE);
	//DrawLineEx(latestNodey.pos, randyNodey.pos, 3, GREEN);

	return;
}