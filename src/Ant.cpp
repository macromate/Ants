#include "Ant.hpp"
#include "NodeManager.hpp"
#include "Node.hpp"
#include "ScriptManager.hpp"
#include "Event.hpp"
#include "Pathfinder.hpp"
#include <cstdlib> // for abs
#include <cassert>

using std::vector;
using ants::Sprite;
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
		float angle = mTargetAngle;
		if (angle == 360) {
			angle = 0;
		}
        if(mMoveTarget && ((spriteRot >= angle 
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
            // entweder script aufrufen
            // oder schauen ob es einen
            // pfad gibt, der noch nicht 
            // fertig ist
            
            if (mPath.empty()) {
                // script zur zielbestimmung verwenden
                using namespace boost::python;
                ScriptManager* sm = ScriptManager::getInstance();
                sm->registerGameObject("self", this);
                sm->startScript("Move");

                // überprüfen ob ziel gesetzt wurde
                if(!mMoveTarget) {
                    // TODO: throw severe error
                }
            } else {
                // ziel wird durch pfad bestimmt
                Node* nextNode = mPath[0];
                mPath.erase(mPath.begin());
                setMoveTarget(nextNode);
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

void Ant::setMoveTarget(Node* node) {
    // wenn das ziel ein nachbarnode ist, dann proceed
    // sonst wird ein pfad erstellt und der erste node darauf
    // als ziel genommen
    
    // TODO: replace check with exception
    assert(getNode() != node);
    
    if (NodeManager::getInstance()->areNeighbours(getNode(), node)) {
        // ziel ist ein nachbar 
        // TODO: überprüfen ob ziel begehbar ist, das sollte eigentlich im skript passieren
        // hier wird dann eine exception geworfen (auf die das script reagieren kann??)
        mMoveTarget = node;
        // ziel direction setzen
        mDirectionToMoveTarget.x = (mMoveTarget->getX() - getNode()->getX()) / PIXELS_PER_NODE;
        mDirectionToMoveTarget.y = (mMoveTarget->getY() - getNode()->getY()) / PIXELS_PER_NODE;
        // relativer Winkel Ameise / Zielnode
        mTargetAngle = getAngleForDirectionVector(mDirectionToMoveTarget);
        if (mTargetAngle == 0) {
            mTargetAngle = 360;
        }
    } else {
        // pfad zu ziel berechnen
        Pathfinder finder(getNode(), node);
        mPath = finder.search();
        assert(!mPath.empty());
        Node* firstNode = mPath[0];
        mPath.erase(mPath.begin());
        setMoveTarget(firstNode);
    }
}
