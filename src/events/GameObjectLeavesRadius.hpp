#ifndef GAMEOBJECT_LEAVES_RADIUS_HPP
#define GAMEOBJECT_LEAVES_RADIUS_HPP

#include "Definitions.hpp"
#include "Event.hpp"

class GameObjectLeavesRadius : public Event {
public:
  GameObjectLeavesRadius(GameObject* centerObject, GameObject* objectInRadius);
  GameObjects getReceivers() const;
  inline GameObject* getCenterObject() const { return mCenterObject; }
  inline GameObject* getLeavingObject() const { return mLeavingObject; }
protected:
  GameObject* mCenterObject;
  GameObject* mLeavingObject;
};


#endif