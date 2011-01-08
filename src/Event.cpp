#include "Event.hpp"

Event::Event(EventType type, const GameObject* singleTarget, const GameObject* sender) : 
mType(type),
mSingleTarget(singleTarget),
mTargets(0),
mSender(sender)
{
}

Event::Event(EventType type, std::vector<GameObject*> targets, GameObject* sender) : 
mType(type),
mSingleTarget(0),
mTargets(targets),
mSender(sender)
{
}
