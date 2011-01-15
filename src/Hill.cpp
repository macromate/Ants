#include "Hill.hpp"
#include "AssetManager.hpp"
#include "Node.hpp"
#include <string>
#include <boost/python.hpp>
#include "ScriptManager.hpp"
#include "EventManager.hpp"
#include "events/GameObjectArrivesInRadius.hpp"

Hill::Hill(Strain strain, Node* node) :
mStrain(strain) {
    GameObject::GameObject();
    setNode(node);
    setRadius(4);
    mSprite = new ants::Sprite();
    std::string imgName;
    switch(mStrain) {
        case StrainYellow: imgName = "hill_yellow.png"; break;
        case StrainRed: imgName = "hill_red.png"; break;
        case StrainBlue: imgName = "hill_blue.png";
    }
    mSprite->SetImage(*AssetManager::getInstance()->getImage(imgName));
    mSprite->SetPosition(mNode->getX(), mNode->getY());
}

void Hill::receiveEvent(Event* event) {
    using namespace boost::python;
    ScriptManager* sm = ScriptManager::getInstance();
    if (event->getEventType() == ETYPE_IN_RADIUS) {
        GameObjectArrivesInRadius* new_event = static_cast<GameObjectArrivesInRadius*>(event);
        sm->registerGameObject("ant", new_event->getObjectInRadius());
        sm->registerGameObject("self", new_event->getCenterObject());
        sm->startScript("ant_arrives_in_radius_of_hill");
    } else if (event->getEventType() == ETYPE_LEAVES_RADIUS) {
      // sm->startScript("ant_leaves_from_radius");
    } else {
        std::cout << "GameObject received unspecified event" << std::endl;
    }
}

Hill::~Hill() {
    if (mSprite) {
        delete mSprite;
        mSprite = 0;
    }
}

