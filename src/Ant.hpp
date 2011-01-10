#pragma once
#ifndef ANT_HPP
#define ANT_HPP

#include "Definitions.hpp"
#include "GameObject.hpp"
#include "Spice.hpp"
#include "Twig.hpp"
#include "AssetManager.hpp"
#include "Pathfinder.hpp"

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

    void setMoveTarget(Node*);
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
	Node* mMoveTarget; // ein angrenzender node, das nächste ziel
    sf::Vector2f mDirectionToMoveTarget;
    float mTargetAngle; // winkel der ameise relativ zum ziel in grad
    Path mPath; // ein path, der nacheinander abgearbeitet wird
private:
    int getAngleForDirectionVector(sf::Vector2f&) const; // in grad
};

#endif
