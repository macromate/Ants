#include "Node.hpp"
#include "GameObject.hpp"
#include "AssetManager.hpp"

#include <algorithm>

using std::vector;

Node::Node(int x, int y) : 
mX(x),
mY(y),
mCost(0.0),
mGameObjects(),
mSprite()
{
    mSprite = new ants::Sprite();
	sf::Image* img = AssetManager::getInstance()->getImage("grass.png");
	img->SetSmooth(false);
    mSprite->SetImage(*img);
	mSprite->SetCenter(0, 0);
    mSprite->SetPosition(x, y);
	mSprite->SetBlendMode(sf::Blend::None);
}

Node::~Node() {
    if(mSprite) {
        delete mSprite;
        mSprite = 0;
    }
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
