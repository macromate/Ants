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
  Eat,
  Take
} AntAction;

class Ant : public GameObject {
public:
	Ant(Strain, Node*);
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
	
    // inline void setCurrentAction(AntAction act) { mCurrentAction = act; }
    void eat(Node*);
    
protected:
	Strain mStrain;
	GameObject* mInventory;
	int mStrength;
	AntAction mCurrentAction;
	Node* mMoveTarget; // ein angrenzender node, das nächste ziel
    Path mPath; // ein path, der nacheinander abgearbeitet wird
    Node* mEatTarget;
    // überprüft ob der sprite richtig zu einem node gedreht ist
    bool doesFaceNode(Node*) const;
    // dreht den sprite in richtung node
    void rotateToFaceNode(Node* node, float deltaTime);
    // gibt einen richtungsvektor zum node an
    sf::Vector2f directionToNode(Node*) const;
private:
    int getAngleToNode(Node*) const; // in grad
};

#endif
