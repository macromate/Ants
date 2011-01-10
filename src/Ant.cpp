#include "Ant.hpp"
#include "NodeManager.hpp"
#include "Node.hpp"
#include "ScriptManager.hpp"
#include "Event.hpp"
#include <cstdlib> // for abs

using std::vector;
using sf::Sprite;
using sf::Vector2f;

Ant::Ant(Strain strain, Coordinate coor) :
    mStrain(strain),
    mInventory(),
    mStrength(1),
    mCurrentAction(Move),
    mMoveTarget()
{
    GameObject::GameObject();
    mSprite = new Sprite();
    mSprite->SetImage(*AssetManager::getInstance()->getImage("ant.png")); // todo: image gets probably copied here
    mSprite->SetPosition(coor.getX(), coor.getY());
    setNode(NodeManager::getInstance()->getNode(coor)); // sets virtual position
    //std::cout << "Ant constructor" << std::endl;
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
void Ant::receiveEvent(Event* event) {
    ScriptManager *sm = ScriptManager::getInstance();
    if (event->getEventType() == ETYPE_ANT_STARTS_ATTACK) {
    // GameObject *defender = event.getTargets()[0];
    // GameObject *attacker = event.getTargets()[1];
    // if (this == defender) {
    //  sm->startScript("ant_is_defending");
    // } else if (this == attacker) {
    //  sm->startScript("ant_is_attacking");
    // }
    } else if (event->getEventType() == ETYPE_INVENTORY_ADD) {
        sm->startScript("ant_sets_inventory");
    } else {
        GameObject::receiveEvent(event);
    }

}

void Ant::setInventory(GameObject* obj) {
    if (!mInventory && this->getNode() == obj->getNode()) {
        mInventory = obj;
        obj->setNode(0);
    // Event event = Event(ETYPE_INVENTORY_ADD, 0, this);
    // EventManager::getInstance()->fire(event);
    }
}

void Ant::emptyInventory() {
    // TODO: Wenn neben einem Ameisenbau, dann dortige Values erhöhen
    mInventory = 0;
}

// wird einmal pro frame aufgerufen
void Ant::handleCurrentAction(float deltaTime) {
    // deltaTime = 1.0f;
    if (mCurrentAction == Move) {
        // wenn das ziel gesetzt ist, dann nur die sprite position updaten
        // wenn das ziel nicht gesetzt ist, dann python script aufrufen um ziel zu setzen
        if(mMoveTarget) {
            // ziel ist vorhanden
            Vector2f direction;
            direction.x = (mMoveTarget->getX() - getNode()->getX()) / PIXELS_PER_NODE;
            direction.y = (mMoveTarget->getY() - getNode()->getY()) / PIXELS_PER_NODE;
			Vector2f oldPosition = mSprite->GetPosition();
            Vector2f newPosition;
            newPosition.x = oldPosition.x + direction.x * deltaTime * 10;
            newPosition.y = oldPosition.y + direction.y * deltaTime * 10;
            mSprite->SetPosition(newPosition);

            // überprüfen ob am ziel angelangt
            Vector2f targetPosition;
            targetPosition.x = mMoveTarget->getX();
            targetPosition.y = mMoveTarget->getY();

            bool isAtTarget = false;

            if (direction.x == 0 && direction.y == -1) {
                // north
                isAtTarget = newPosition.x == targetPosition.x &&
                    newPosition.y <= targetPosition.y;
            } else if (direction.x == 0 && direction.y == 1) {
                // south
                isAtTarget = newPosition.x == targetPosition.x &&
                    newPosition.y >= targetPosition.y;
            } else if (direction.x == 1 && direction.y == 0) {
                // east
                isAtTarget = newPosition.x >= targetPosition.x &&
                    newPosition.y == targetPosition.y;
            } else if (direction.x == -1 && direction.y == 0) {
                // west
                isAtTarget = newPosition.x <= targetPosition.x &&
                    newPosition.y == targetPosition.y;
            } else if (direction.x == 1 && direction.y == -1) {
                // north east
                isAtTarget = newPosition.x >= targetPosition.x &&
                    newPosition.y <= targetPosition.y;
            } else if (direction.x == -1 && direction.y == -1) {
                // north west
                isAtTarget = newPosition.x <= targetPosition.x &&
                    newPosition.y <= targetPosition.y;
            } else if (direction.x == 1 && direction.y == 1) {
                // south east
                isAtTarget = newPosition.x >= targetPosition.x &&
                    newPosition.y >= targetPosition.y;
            } else if (direction.x == -1 && direction.y == 1) {
                // south west
                isAtTarget = newPosition.x <= targetPosition.x &&
                    newPosition.y >= targetPosition.y;
            }

            if (isAtTarget) {
                setNode(mMoveTarget);
                mMoveTarget = 0;
            }
        } else {
            // ziel ist nicht vorhanden
            using namespace boost::python;
            ScriptManager* sm = ScriptManager::getInstance();
            sm->registerGameObject("self", this);
            sm->startScript("Move");
        }
    }
}

void Ant::setNode(Node* node) {
    GameObject::setNode(node);
    mSprite->SetX(node->getX());
    mSprite->SetY(node->getY());
}
