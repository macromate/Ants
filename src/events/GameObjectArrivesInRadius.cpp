#include "GameObjectArrivesInRadius.hpp"
GameObjectArrivesInRadius::GameObjectArrivesInRadius(GameObject* centerObject, 
                                                     GameObject* objectInRadius) :
mCenterObject(centerObject),
mObjectInRadius(objectInRadius)
{
  setEventType(ETYPE_IN_RADIUS);
}

// TODO: return a c array here ??
GameObjects GameObjectArrivesInRadius::getReceivers() const {
  GameObjects objects;
  // objects.push_back(mCenterObject);
  objects.push_back(mObjectInRadius);
  return objects;
}
