#include "Ant.hpp"
#include "NodeManager.hpp"
#include "Node.hpp"
#include "ScriptManager.hpp"

using std::vector;
using sf::Sprite;

Ant::Ant(Strain strain, Coordinate coor) :
mStrain(strain),
mInventory(),
mStrength(1)
{
	GameObject::GameObject();
	mSprite = new Sprite();
	mSprite->SetImage(*AssetManager::getInstance()->getImage("ant.png")); // todo: image gets probably copied here
	mSprite->SetPosition(ConvertXCoordinate(coor.getX()), ConvertYCoordinate(coor.getY()));
	setNode(NodeManager::getInstance()->getNode(coor)); // sets virtual position
	//std::cout << "Ant constructor" << std::endl;
	EventManager::getInstance()->addListener(this, ETYPE_IN_RADIUS);
	EventManager::getInstance()->addListener(this, ETYPE_LEAVES_RADIUS);
	EventManager::getInstance()->addListener(this, ETYPE_INVENTORY_ADD);
}

Ant::Ant(const Ant& ant) {
  GameObject::GameObject();
	std::cout << "Ant copy constructor was called" << std::cout;
}

Ant::~Ant() {
	//std::cout << "Ant destructor" << std::endl;
	if (mSprite) {
		delete mSprite;
		mSprite = 0;
	}
}

/* this is were scripts are set up and started according to the events the ant receives */
void Ant::receiveEvent(Event event) {
	ScriptManager *sm = ScriptManager::getInstance();
	if(event.getEventType() == ETYPE_IN_RADIUS) {
//		GameObject* gb = const_cast<GameObject*>(event.getSingleTarget());
//		Node* node = gb->getNode();
//		Coordinate coor = node->getCoordinate();
//		sm->getEngine()->RegisterGlobalProperty("Coordinate antCoordinate", &coor);
		sm->startScript("ant_arrives_in_radius");
	} else if (event.getEventType() == ETYPE_ANT_STARTS_ATTACK) {
		GameObject *defender = event.getTargets()[0];
		GameObject *attacker = event.getTargets()[1];
		if (this == defender) {
			sm->startScript("ant_is_defending");
		} else if (this == attacker) {
			sm->startScript("ant_is_attacking");
		}
	} else if (event.getEventType() == ETYPE_INVENTORY_ADD) {
		sm->startScript("ant_sets_inventory");
	} else {
		GameObject::receiveEvent(event);
	}

}

void Ant::setInventory(GameObject* obj) {
	if (!mInventory && this->getNode() == obj->getNode()) {
		mInventory = obj;
		obj->setNode(0);
		Event event = Event(ETYPE_INVENTORY_ADD, 0, this);
		EventManager::getInstance()->fire(event);
	}
}

void Ant::emptyInventory() {
	// TODO: Wenn neben einem Ameisenbau, dann dortige Values erhöhen
	mInventory = 0;
}