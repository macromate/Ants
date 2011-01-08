#include "Node.hpp"
#include "GameObject.hpp"

#include <algorithm>

using std::vector;

Coordinate MakeCoordinate(int x, int y) {
	Coordinate tmp;
	tmp.x = x;
	tmp.y = y;
	return tmp;
}

int ConvertXCoordinate(int x) {
	return x * 32;
}

int ConvertYCoordinate(int y) {
	return y * 32;
}

Node::Node(int x, int y) : 
mX(x),
mY(y),
mCost(0.0),
mGameObjects()
{
}

Node::~Node() {
}

void Node::addGameObject(GameObject* gameObject) {
	mGameObjects.push_back(gameObject);
	mCost += gameObject->getCost();
}

void Node::removeGameObject(GameObject* gameObject) {
	mGameObjects.erase(std::find(mGameObjects.begin(), mGameObjects.end(), gameObject));
	mCost -= gameObject->getCost();
}

bool Node::isAccessible() const {
	GameObjects::const_iterator i = mGameObjects.begin();
	for (; i < mGameObjects.end(); i++) {
		if (!(*i)->isAccessible())
			return false;
	}
	return true;
}

bool Node::operator==(Node* obj) const {
	return mX == obj->getX() && mY == obj->getY();
}