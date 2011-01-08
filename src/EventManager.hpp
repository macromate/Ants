#pragma once
#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP
#include "Event.hpp"
#include "GameObject.hpp"
#include <map>
#include <vector>

class EventManager {
public:
	static EventManager *getInstance();
	~EventManager();
	void addListener(GameObject*, EventType); // TODO: Add removeListener method
	void fire(Event);
private:
	EventManager();
	EventManager(const EventManager&);
	static EventManager *mInstance;
	std::map<EventType, std::vector<GameObject*> > mListeners;
};
#endif
