#pragma once
#ifndef	PATHFINDER_HPP
#define PATHFINDER_HPP

#include <vector>
#include <map>
#include <cstdlib> // for abs
#include <algorithm>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "NodeManager.hpp"

class PathfinderNode;
typedef	boost::shared_ptr<PathfinderNode> PathfinderNodePtr;

class PathfinderNode {
public:
	PathfinderNode(Node* node);
	PathfinderNode(Node* node, PathfinderNodePtr parent, Node* target);
	~PathfinderNode();
	inline void setParent(PathfinderNodePtr parent) { mParent = parent; calculateCosts(); }
	inline PathfinderNodePtr getParent() const { return mParent; }
	inline Node* getNode() const { return mNode; }
	inline float getGCost() const { return mGCost; }
	inline float getFCost() const { return mGCost + mHCost; }
	bool isEqual(PathfinderNodePtr);
protected:
	Node *mNode;
	PathfinderNodePtr mParent;
	float mGCost;
	float mHCost;
	Node* mTarget;
	void calculateCosts();
};

typedef	boost::shared_ptr<PathfinderNode> PathfinderNodePtr;
typedef std::vector<PathfinderNodePtr> PathfinderNodes;
typedef std::vector<Node*> Path;
typedef std::map<float, PathfinderNodePtr> PathfinderNodeMap;

class NodeQueue {
public:
	NodeQueue();
	PathfinderNodePtr top() const;
	void pop();
	bool includes(PathfinderNodePtr node) const;
	bool empty() const;
	void push(PathfinderNodePtr node);
	void update(PathfinderNodePtr node);
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
};
#endif
