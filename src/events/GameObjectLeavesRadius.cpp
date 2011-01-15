#include "GameObjectLeavesRadius.hpp"
GameObjectLeavesRadius::GameObjectLeavesRadius(GameObject* centerObject, 
                                               GameObject* leavingObject) :
mCenterObject(centerObject),
mLeavingObject(leavingObject)
{
  setEventType(ETYPE_LEAVES_RADIUS);
}

// TODO: return a c array here ??
GameObjects GameObjectLeavesRadius::getReceivers() const {
  GameObjects objects;
  objects.push_back(mLeavingObject);
  return objects;
}
