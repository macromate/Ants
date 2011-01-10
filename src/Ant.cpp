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
    mMoveTarget(),
    mDirectionToMoveTarget(),
    mTargetAngle()
{
    GameObject::GameObject();
    mSprite = new Sprite();
    mSprite->SetImage(*AssetManager::getInstance()->getImage("ant.png")); // todo: image gets probably copied here
    mSprite->SetPosition(coor.getX(), coor.getY());
    mSprite->SetCenter(16.0f, 16.0f); // sprite is 32 pixel
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
    // deltaTime = 0.01f;
    if (mCurrentAction == Move) {
        // wenn das ziel gesetzt ist, dann nur die sprite position updaten
        // wenn das ziel nicht gesetzt ist, dann python script aufrufen um ziel zu setzen
		int spriteRot = static_cast<int>(mSprite->GetRotation());
		int nextDirAngle = mTargetAngle + 45;
		if (nextDirAngle > 360) {
			nextDirAngle = 45;
		}
        if(mMoveTarget && ((spriteRot >= mTargetAngle 
						   && spriteRot < nextDirAngle)
						   || (spriteRot == 180 && mTargetAngle == 0))) {
            // ziel ist vorhanden
			Vector2f oldPosition = mSprite->GetPosition();
            Vector2f newPosition;
            newPosition.x = oldPosition.x + mDirectionToMoveTarget.x * deltaTime * 10;
            newPosition.y = oldPosition.y + mDirectionToMoveTarget.y * deltaTime * 10;
            mSprite->SetPosition(newPosition);

            // überprüfen ob am ziel angelangt
            Vector2f targetPosition;
            targetPosition.x = mMoveTarget->getX();
            targetPosition.y = mMoveTarget->getY();

            bool isAtTarget = false;
            
            // bewegungsrichtung kommt eine andere routine zum einsatz
            switch(directionForVector(mDirectionToMoveTarget)) {
                case North:
                    isAtTarget = newPosition.x == targetPosition.x &&
                        newPosition.y <= targetPosition.y;
                    break;
                case South:
                    isAtTarget = newPosition.x == targetPosition.x &&
                        newPosition.y >= targetPosition.y;
                    break;
                case East:
                    isAtTarget = newPosition.x >= targetPosition.x &&
                        newPosition.y == targetPosition.y;
                    break;
                case West:
                    isAtTarget = newPosition.x <= targetPosition.x &&
                        newPosition.y == targetPosition.y;
                    break;
                case NorthEast:
                    isAtTarget = newPosition.x >= targetPosition.x &&
                        newPosition.y <= targetPosition.y;
                    break;
                case NorthWest:
                    isAtTarget = newPosition.x <= targetPosition.x &&
                        newPosition.y <= targetPosition.y;
                    break;
                case SouthEast:
                    isAtTarget = newPosition.x >= targetPosition.x &&
                        newPosition.y >= targetPosition.y;
                    break;
                case SouthWest:
                    isAtTarget = newPosition.x <= targetPosition.x &&
                        newPosition.y >= targetPosition.y;
            }
            
            if (isAtTarget) {
                setNode(mMoveTarget);
                mMoveTarget = 0;
            }
            
        } else if(mMoveTarget) {
            // ziel gefunden, muss aber noch richtig drehen
            mSprite->Rotate(mTargetAngle * deltaTime);
        } else {
            // ziel ist nicht vorhanden
            using namespace boost::python;
            ScriptManager* sm = ScriptManager::getInstance();
            sm->registerGameObject("self", this);
            sm->startScript("Move");

            // überprüfen ob ziel gesetzt wurde
            if(!mMoveTarget) {
                // TODO: throw severe error
            }

            // überprüfen ob ziel ein nachbarnode ist

            // überprüfen ob ziel begehbar ist
            // sollte eigentlich im skript passieren
            // hier wird dann ein fehler angezeigt

            // ziel direction setzen
            mDirectionToMoveTarget.x = (mMoveTarget->getX() - getNode()->getX()) / PIXELS_PER_NODE;
            mDirectionToMoveTarget.y = (mMoveTarget->getY() - getNode()->getY()) / PIXELS_PER_NODE;

            // relativer Winkel Ameise / Zielnode
            mTargetAngle = getAngleForDirectionVector(mDirectionToMoveTarget);
			if (mTargetAngle == 0) {
				mTargetAngle = 360;
			}
        }
    }
}

void Ant::setNode(Node* node) {
    GameObject::setNode(node);
    mSprite->SetX(node->getX());
    mSprite->SetY(node->getY());
}

int Ant::getAngleForDirectionVector(Vector2f &direction) const {
    switch(directionForVector(direction)) {
        case North: return 0;
        case South: return 180;
        case East: return 270;
        case West: return 90;
        case NorthEast: return 315;
        case NorthWest: return 45;
        case SouthEast: return 225;
        case SouthWest: return 135;
    }
    return 0;
}
