#pragma once
#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP
#include "Event.hpp"
#include <map>
#include <vector>

class GameObject;

class EventManager {
public:
	static EventManager *getInstance();
	~EventManager();
	void fire(Event*);
private:
	EventManager();
	EventManager(const EventManager&);
	static EventManager *mInstance;
};
#endif
