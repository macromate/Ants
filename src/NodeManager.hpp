#pragma once
#ifndef NODE_MANAGER_HPP
#define NODE_MANAGER_HPP

#include "Definitions.hpp"
#include "Node.hpp"

#include <vector>
#include <iostream>


#define PIXELS_PER_NODE 32

typedef std::vector<Node*> Nodes;

class NodeManager {
public:
	static NodeManager *getInstance();
	virtual ~NodeManager();
	void addNode(Node* node);
	Node* getNode(int x, int y) const;
	Node* getNode(Coordinate coor) const;
	Nodes getNeighbourNodes(Node*) const;
	Nodes getNodesInRadius(Node* node, int radius) const;
	inline Nodes getNodes() const { return mNodes; }
	bool isIncluded(Coordinate coor) const;
    bool isIncluded(sf::Vector2f) const;
	bool areNodesDiagonal(Node*, Node*) const;
    bool areNeighbours(Node*, Node*) const;
    Node* getRandomNode() const;
    Node* getEmptyRandomNode() const;
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
