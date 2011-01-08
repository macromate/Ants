#pragma once
#ifndef EVENT_HPP
#define EVENT_HPP

#include "Definitions.hpp"

#include <vector>

class GameObject;
class EventManager;

class Event {
public:
	Event(EventType, const GameObject* singleTarget, const GameObject* const sender);
	Event(EventType, std::vector<GameObject*> targets, GameObject* sender);
	EventType getEventType() const { return mType; }
	const GameObject* getSingleTarget() const { return mSingleTarget; };
	GameObjects getTargets() const { return mTargets; }
	const GameObject* getSender() const { return mSender; }
private:
	EventType mType;
	const GameObject* mSingleTarget;
	std::vector<GameObject*> mTargets;
	const GameObject *mSender;
};
#endif
