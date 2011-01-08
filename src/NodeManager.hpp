#pragma once
#ifndef NODE_MANAGER_HPP
#define NODE_MANAGER_HPP

#include <vector>
#include <iostream>
#include "Node.hpp"

typedef std::vector<Node*> Nodes;

class NodeManager {
public:
	static NodeManager *getInstance();
	virtual ~NodeManager();
	void addNode(Node* node);
	Node* getNode(int x, int y);
	Node* getNode(Coordinate coor);
	Nodes getNeighbourNodes(Node*);
	Nodes getNodesInRadius(Node* node, int radius);
	inline Nodes getNodes() const { return mNodes; }
	bool isIncluded(Coordinate coor);
	bool areNodesDiagonal(Node*, Node*);
protected:
	static NodeManager* mInstance;
	Nodes mNodes;
	// das spielfeld kann theoretisch auch minus koordinaten annehmen
	int mLeftBorder, mRightBorder, mTopBorder, mBottomBorder;
private:
	NodeManager();
	NodeManager(const NodeManager&);
};

#endif
