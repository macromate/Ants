#pragma once
#ifndef	PATHFINDER_HPP
#define PATHFINDER_HPP

#include <vector>
#include <map>
#include <cstdlib> // for abs
#include <algorithm>
#include <iostream>
#include "NodeManager.hpp"

class PathfinderNode {
public:
	PathfinderNode(Node *node);
	PathfinderNode(Node* node, PathfinderNode* parent, Node* target);
	~PathfinderNode();
	void setParent(PathfinderNode* parent) { mParent = parent; calculateCosts(); }
	PathfinderNode* getParent() const { return mParent; }
	Node* getNode() const { return mNode; }
	float getGCost() const { return mGCost; }
	float getFCost() const { return mGCost + mHCost; }
	bool isEqual(PathfinderNode* node);
protected:
	Node *mNode;
	PathfinderNode *mParent;
	float mGCost;
	float mHCost;
	Node* mTarget;
	void calculateCosts();
};

typedef std::vector<PathfinderNode*> PathfinderNodes;
typedef std::vector<Coordinate> Path;
typedef std::map<float, PathfinderNode*> PathfinderNodeMap;

class NodeQueue {
public:
	NodeQueue();
	PathfinderNode* top() const;
	void pop();
	bool includes(PathfinderNode* node) const;
	bool empty() const;
	void push(PathfinderNode* node);
	void update(PathfinderNode* node);
protected:
	PathfinderNodeMap mMap;
};

class Pathfinder {
public:
	Pathfinder(Node* start, Node* target);
	~Pathfinder();
	Path search();
private:
	Node* mStart;
	Node* mTarget;
	NodeQueue mOpenList;
	PathfinderNodes mClosedList;
	void addNeighbourNodesToOpenList(PathfinderNode*);
};
#endif
