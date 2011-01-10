#pragma once
#ifndef ANT_HPP
#define ANT_HPP

#include "Definitions.hpp"
#include "GameObject.hpp"
#include "Spice.hpp"
#include "Twig.hpp"
#include "AssetManager.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

class Node;

typedef enum {
  Move,
  Eat
} AntAction;

class Ant : public GameObject {
public:
	Ant(Strain, Coordinate);
	Ant(const Ant& ant);
	virtual ~Ant();
	virtual void receiveEvent(Event*);
	inline bool isAccessible() const { return true; }

    inline Strain getStrain() { return mStrain; }

    void handleCurrentAction(float deltaTime);

    inline void setMoveTarget(Node* node) { mMoveTarget = node; }
    inline Node* getMoveTarget() const { return mMoveTarget; }

	void setNode(Node*);

	// inventory methods
	void emptyInventory();
	void setInventory(GameObject*);

protected:
	Strain mStrain;
	GameObject* mInventory;
	int mStrength;
	AntAction mCurrentAction;
	Node* mMoveTarget;
};

#endif
