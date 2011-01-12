#include "GameObject.hpp"
#include "Node.hpp"
#include "NodeManager.hpp"
#include "ScriptManager.hpp"
#include "Pathfinder.hpp"
#include "EventManager.hpp"

#include "events/GameObjectArrivesInRadius.hpp"

#include <iostream>
#include <algorithm>

int GameObject::sHighestId = 1;

using std::find;

GameObject::GameObject() :
mCost(0.0),
mNode(),
mRadius(1),
mObjectsInRadius(),
mSprite(0),
mId(sHighestId) {
//	std::cout << "constructor gameobject" << std::endl;
  sHighestId++;
}

GameObject::GameObject(Node* node) :
mCost(0.0),
mNode(),
mRadius(1),
mObjectsInRadius(),
mSprite(0),
mId(sHighestId) {
//	std::cout << "constructor gameobject" << std::endl;
  sHighestId++;
  setNode(node);
}

GameObject::~GameObject() {
//	std::cout << "destructor gameobject" << std::endl;
}

GameObject::GameObject(const GameObject& obj) :
mCost(mCost),
mNode(mNode),
mRadius(mRadius),
mObjectsInRadius() {
//	std::cout << "copy constructor gameobject" << std::endl;
}

void GameObject::setNode(Node* node) {
	if (mNode) // wenn ein voriger node existiert
		mNode->removeGameObject(this); // dann wird dieses objekt entfernt
	mNode = node;
	if (node) // im inventar haben objekte keinen node (Nullpointer)
		mNode->addGameObject(this);
}

void GameObject::receiveEvent(Event* event) {
  using namespace boost::python;
  ScriptManager* sm = ScriptManager::getInstance();
  if (event->getEventType() == ETYPE_IN_RADIUS) {
    GameObjectArrivesInRadius* new_event = static_cast<GameObjectArrivesInRadius*>(event);
    sm->registerGameObject("ant", new_event->getObjectInRadius());
    sm->registerGameObject("self", new_event->getCenterObject());
    sm->startScript("ant_arrives_in_radius");
  } else if (event->getEventType() == ETYPE_LEAVES_RADIUS) {
	  sm->startScript("ant_leaves_from_radius");
	} else {
		std::cout << "GameObject received unspecified event" << std::endl;
	}
}

void GameObject::trigger() {
	if (mNode) {
		// alle gameobjects im radius finden
		GameObjects objects;
		Nodes neighbours = NodeManager::getInstance()->getNodesInRadius(mNode, mRadius);
		Nodes::iterator i = neighbours.begin();
		for (; i < neighbours.end(); i++) {
			GameObjects tmp = (*i)->getGameObjects();
			objects.insert(objects.begin(), tmp.begin(), tmp.end());
		}
		// überprüfen ob neue Objekte dazugekommen sind
		if (!objects.empty()) {
			GameObjects::iterator n = objects.begin();
			for (; n < objects.end(); n++) {
				if (find(mObjectsInRadius.begin(), mObjectsInRadius.end(), *n) == mObjectsInRadius.end()) {
					// Objekt befindet sich zum ersten mal im Radius
          GameObjectArrivesInRadius* event = new GameObjectArrivesInRadius(this, (*n));
					EventManager::getInstance()->fire(event);
					// Objekt speichern, damit beim Triggern im nächsten Frame nicht wieder das Event gesendet wird
					mObjectsInRadius.push_back(*n);
				} else {
					// Objekt befindet sich schon länger im Radius
					// Event wurde schon getriggert -> jetzt nicht mehr triggern
				}
			}
		}
    // // überprüfen ob objekte nicht mehr da sind
    // GameObjects::iterator n = mObjectsInRadius.begin();
    // for (; n < mObjectsInRadius.end(); n++) {
    //  if (objects.empty() || // kein einzeiges objekt mehr in umgebung
    //    find(objects.begin(), objects.end(), *n) == objects.end()) {
    //    // objekt ist verschwunden
    //    // event darüber senden
    //    GameObjects targets;
    //    targets.push_back(this);
    //    targets.push_back(*n);
    //    Event event = Event(ETYPE_LEAVES_RADIUS, targets, this);
    //    EventManager::getInstance()->fire(event);
    //    // und aus dem zwischenspeicher entfernen
    //    mObjectsInRadius.erase(find(mObjectsInRadius.begin(), mObjectsInRadius.end(), *n));
    //  }
    // }
	} // if(mNode)
}

bool GameObject::isAccessible() const {
	return false;
}
