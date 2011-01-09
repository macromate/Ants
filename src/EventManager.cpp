#include "EventManager.hpp"
#include "GameObject.hpp"

using std::map;
using std::pair;
using std::vector;

EventManager *EventManager::mInstance = 0;

EventManager *EventManager::getInstance() {
	if (!mInstance) {
		mInstance = new EventManager();
	}
	
	return mInstance;
}

EventManager::EventManager() {
	// private constructor
}

EventManager::EventManager(const EventManager& ref)
{
	// private copy constructor
}

EventManager::~EventManager() {
	delete mInstance;
}

void EventManager::fire(Event& event) {
  GameObjects receivers = event.getReceivers();
  GameObjects::iterator i = receivers.begin();
  for(; i < receivers.end(); i++) {
    (*i)->receiveEvent(event);
  }
}
