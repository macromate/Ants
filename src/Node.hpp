#pragma once
#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <iostream>

#include "Definitions.hpp"
#include "Sprite.hpp"

class GameObject;

class Node {
public:
	Node(int x, int y);
	virtual ~Node();
	inline int getX() const { return mX; } // TODO: make function inline
	inline int getY() const { return mY; }
	inline float getCost() const { return mCost; }
    inline bool isEmpty() const { return mGameObjects.empty(); }
	void addGameObject(GameObject* gameObject);
	void removeGameObject(GameObject* gameObject);
	inline GameObjects getGameObjects() { return mGameObjects; }
	
    inline ants::Sprite* getSprite() const { return mSprite; }
	
	bool isAccessible() const;
	Coordinate getCoordinate() const { return Coordinate(getX(), getY()); }
	
	bool operator==(Node* obj) const;
protected:
	int mX, mY;
	float mCost;
	// game objects associated with this node, for determinating the costs, 
	GameObjects mGameObjects;
    ants::Sprite* mSprite;
};

#endif
