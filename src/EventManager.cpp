#include "EventManager.hpp"
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

EventManager::EventManager() : 
mListeners() {
	// private constructor
}

EventManager::EventManager(const EventManager& ref) :
mListeners()
{
	// private copy constructor
}

EventManager::~EventManager() {
	delete mInstance;
}

/* Adds the listener for this type if it doesn't exist */
void EventManager::addListener(GameObject* listener, EventType type) {
	if(mListeners.count(type) > 0) {
		// TODO: Add check if listener is already registered
		mListeners[type].push_back(listener);
	} else {
		vector<GameObject*> listeners = vector<GameObject*>();
		listeners.push_back(listener);
		mListeners.insert(pair<EventType, vector<GameObject*> >(type, listeners));
	}
}

/* Searches for listeners and then sends the event to them 
   TODO: Event sollte mit smart pointer übergeben werden 
   Die jetzige implementierung sendet events nur an die objekte
   die das target sind */
void EventManager::fire(Event event) {
	vector<GameObject*>::iterator i = mListeners[event.getEventType()].begin();
	// looping through all listeners for this event type
	for (; i < mListeners[event.getEventType()].end(); i++) {
		// check if the listener is the target
		if (event.getSingleTarget()) {
			// this event has only one target
			if ((*i) == event.getSingleTarget())
				(*i)->receiveEvent(event);
		} else if(!event.getTargets().empty()) {
			// this event has multiple targets
			bool isTarget = false;
			vector<GameObject*>::iterator n = event.getTargets().begin();
			for (; n < event.getTargets().end(); n++) {
				if ((*i) == (*n)) {
					isTarget = true;
				}
			}
			if (isTarget) {
				(*i)->receiveEvent(event);
			}
		} else {
			// this event has no targets, send anyway
			(*i)->receiveEvent(event);
		}


	}
}
