#include "Ant.hpp"
#include "NodeManager.hpp"
#include "Node.hpp"
#include "ScriptManager.hpp"
#include "GameObjectManager.hpp"
#include "Event.hpp"
#include "Pathfinder.hpp"
#include <cstdlib> // for abs
#include <cassert>

using std::vector;
using ants::Sprite;
using sf::Vector2f;

Ant::Ant(Strain strain, Node* node) :
    GameObject(node),
    mStrain(strain),
    mInventory(),
    mStrength(1),
    mCurrentAction(Move),
    mMoveTarget(0),
    mEatTarget(0)
{
    mSprite = new Sprite();
    mSprite->SetImage(*AssetManager::getInstance()->getImage("ant.png")); // todo: image gets probably copied here
    mSprite->SetPosition(node->getX(), node->getY());
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
    } else if (event->getEventType() == ETYPE_ANT_MOVES) {
        AntMoves* new_event = static_cast<AntMoves*>(event);
        sm->registerGameObject("self", new_event->getAnt());
        sm->startScript("ant_has_moved");
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
        if(mMoveTarget && doesFaceNode(mMoveTarget)) {
            // ziel ist vorhanden
			Vector2f oldPosition = mSprite->GetPosition();
            Vector2f newPosition;
            Vector2f direction = directionToNode(mMoveTarget);
            newPosition.x = oldPosition.x + direction.x * deltaTime * 10;
            newPosition.y = oldPosition.y + direction.y * deltaTime * 10;
            mSprite->SetPosition(newPosition);

            // überprüfen ob am ziel angelangt
            Vector2f targetPosition;
            targetPosition.x = mMoveTarget->getX();
            targetPosition.y = mMoveTarget->getY();

            bool isAtTarget = false;
            
            // bewegungsrichtung kommt eine andere routine zum einsatz
            switch(directionForVector(directionToNode(mMoveTarget))) {
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
                
                // event senden
                AntMoves* event = new AntMoves(this);
                EventManager::getInstance()->fire(event);
            }
            
        } else if (mMoveTarget) {
            // ziel gefunden, muss aber noch richtig drehen
            rotateToFaceNode(mMoveTarget, deltaTime);
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

                // überprüfen ob ziel vom skript gesetzt wurde
                assert(mMoveTarget);
            } else {
                // ziel wird durch pfad bestimmt
                Node* nextNode = mPath[0];
                mPath.erase(mPath.begin());
                setMoveTarget(nextNode);
            }
        }
    } // if (mCurrentAction == Move)
    else if (mCurrentAction == Eat) {
        assert(mEatTarget);
        if (doesFaceNode(mEatTarget)) {
            // actually eat the spice at the target node
            // TODO: nur das futter entfernen
			GameObjects objects = mEatTarget->getGameObjects();
			assert(!objects.empty());
            Spice* spice = static_cast<Spice*>(objects.at(0));
            GameObjectManager::getInstance()->deleteSpice(spice);
            mStrength++;
            std::cout << "Gained one strength point" << std::endl;
            mCurrentAction = Move;
        } else {
            rotateToFaceNode(mEatTarget, deltaTime);
        }
    } // if (mCurrentAction == Eat)
}

void Ant::setNode(Node* node) {
    GameObject::setNode(node);
    mSprite->SetX(node->getX());
    mSprite->SetY(node->getY());
}

int Ant::getAngleToNode(Node* node) const {
    sf::Vector2f direction = directionToNode(node);
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

void Ant::eat(Node* node) {
    // TODO: replace check with exception
    assert(NodeManager::getInstance()->areNeighbours(getNode(), node));
    // todo: assert that there is actually food on this node
    mCurrentAction = Eat;
    mEatTarget = node;
}

bool Ant::doesFaceNode(Node* node) const {
    // relativer Winkel Ameise / node
    float targetAngle = getAngleToNode(node);
   	int spriteRot = static_cast<int>(mSprite->GetRotation());
	int nextDirAngle = targetAngle + 45;
	if (nextDirAngle > 360) {
		nextDirAngle = 45;
	}
	float angle = targetAngle;
	if (angle == 360) {
		angle = 0;
	}
	return (spriteRot >= angle && spriteRot < nextDirAngle)
            || (spriteRot == 180 && targetAngle == 0);
}

void Ant::rotateToFaceNode(Node* node, float deltaTime) {
    float targetAngle = getAngleToNode(node);
    mSprite->Rotate(targetAngle * deltaTime);
}

// TODO: eine referenz zurückgeben??
sf::Vector2f Ant::directionToNode(Node* node) const {
    sf::Vector2f direction;
    direction.x = (node->getX() - getNode()->getX()) / PIXELS_PER_NODE;
    direction.y = (node->getY() - getNode()->getY()) / PIXELS_PER_NODE;
    return direction;
}
