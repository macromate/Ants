#ifndef GAMEOBJECT_ARRIVES_IN_RADIUS_HPP
#define GAMEOBJECT_ARRIVES_IN_RADIUS_HPP

class GameObject;

#include "Event.hpp"

class GameObjectArrivesInRadius : public Event {
public:
  GameObjectArrivesInRadius(GameObject* centerObject, GameObject* objectInRadius);
  GameObjects getReceivers() const;
  inline GameObject* getCenterObject() const { return mCenterObject; }
  inline GameObject* getObjectInRadius() const { return mObjectInRadius; }
protected:
  GameObject* mCenterObject; // the object that "sees" the other one
  GameObject* mObjectInRadius; // the object that is seen  
};


#endif