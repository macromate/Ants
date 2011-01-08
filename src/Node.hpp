#pragma once
#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <iostream>

#include "Definitions.hpp"

class GameObject;

class Node {
public:
	Node(int x, int y);
	virtual ~Node();
	inline int getX() const { return mX; } // TODO: make function inline
	inline int getY() const { return mY; }
	inline float getCost() const { return mCost; }
	
	void addGameObject(GameObject* gameObject);
	void removeGameObject(GameObject* gameObject);
	inline GameObjects getGameObjects() { return mGameObjects; }
	
	bool isAccessible() const;
	Coordinate getCoordinate() const { return MakeCoordinate(getX(), getY()); }
	
	bool operator==(Node* obj) const;
protected:
	int mX, mY;
	float mCost;
	// game objects associated with this node, for determinating the costs, 
	//NOTE: vector may be not the best way to do this because objects are often deleted from this
	GameObjects mGameObjects;
};

#endif
