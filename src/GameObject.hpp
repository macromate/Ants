#pragma once
#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include "Definitions.hpp"
#include "EventManager.hpp"

#include "Sprite.hpp"

class Node;

class GameObject {
public:
	GameObject();
    GameObject(Node*);
	GameObject(const GameObject&);
	virtual ~GameObject();
	inline int getId() const { return mId; }
	
	void setCost(float cost) { mCost = cost; }
	inline float getCost() const { return mCost; }
	
	void setNode(Node* node);
	inline Node* getNode() const { return mNode; }
	
	inline void setRadius(int radius) { mRadius = radius; }
	inline int getRadius() const { return mRadius; }
	
	inline ants::Sprite* getSprite() const { return mSprite; }
	inline bool hasSprite() const { return !!(mSprite); }
	
    inline bool isSelected() const { return mIsSelected; }
    inline void setSelectionStatus(bool status) { mIsSelected = status; }
    virtual inline sf::Color getSelectionColor() const { return sf::Color::Magenta; }
	
	// checks all event triggers
	void trigger();
	virtual void receiveEvent(Event*);
	virtual bool isAccessible() const;
	
    GameObjects getObjectsInRadius() const;
	
protected:
	float mCost;
	Node *mNode;
	int mRadius;
	GameObjects mObjectsInRadius;
	ants::Sprite* mSprite;
	int mId;
    static int sHighestId;
    bool mIsSelected;
};

#endif
