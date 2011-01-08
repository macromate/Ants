#include "GameObject.hpp"
#include "Node.hpp"
#include "NodeManager.hpp"
#include "ScriptManager.hpp"
#include "Pathfinder.hpp"

#include <iostream>
#include <algorithm>

using std::find;

GameObject::GameObject() :
mCost(0.0),
mNode(),
mRadius(1),
mObjectsInRadius(),
mSprite(0) {
//	std::cout << "constructor gameobject" << std::endl;
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

void GameObject::receiveEvent(Event event) {
	if (event.getEventType() == ETYPE_LEAVES_RADIUS) { 
//		GameObject* obj1 = event.getTargets()[0];
		GameObject* obj2 = event.getTargets()[1];
		//if (this == obj2) {
			ScriptManager::getInstance()->startScript("ant_leaves_from_radius");
			// find path to new position
//			Pathfinder finder(this->getNode(), obj2->getNode());
//			Path path = finder.search();
//			Path::iterator i = path.begin();
//			for (; i < path.end(); i++) {
//				std::cout << (*i).x << "/" << (*i).y << std::endl;
//		//	}
	//	}
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
					Event event = Event(ETYPE_IN_RADIUS, *n, this);
					EventManager::getInstance()->fire(event);
					// Objekt speichern, damit beim Triggern im nächsten Frame nicht wieder das Event gesendet wird
					mObjectsInRadius.push_back(*n);
				} else {
					// Objekt befindet sich schon länger im Radius
					// Event wurde schon getriggert -> jetzt nicht mehr triggern
				}
				
			}
		}
		// überprüfen ob objekte nicht mehr da sind
		GameObjects::iterator n = mObjectsInRadius.begin();
		for (; n < mObjectsInRadius.end(); n++) {
			if (objects.empty() || // kein einzeiges objekt mehr in umgebung
				find(objects.begin(), objects.end(), *n) == objects.end()) {
				// objekt ist verschwunden
				// event darüber senden
				GameObjects targets;
				targets.push_back(this);
				targets.push_back(*n);
				Event event = Event(ETYPE_LEAVES_RADIUS, targets, this);
				EventManager::getInstance()->fire(event);
				// und aus dem zwischenspeicher entfernen
				mObjectsInRadius.erase(find(mObjectsInRadius.begin(), mObjectsInRadius.end(), *n));
			}
		}
	} // if(mNode)
}

bool GameObject::isAccessible() const {
	return false;
}
