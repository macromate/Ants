#include "Spice.hpp"
#include "NodeManager.hpp"
#include "AssetManager.hpp"
// #include "Sprite.hpp"

Spice::Spice(Node* node) :
GameObject(node) {
	mSprite = new ants::Sprite();
	mSprite->SetImage(*AssetManager::getInstance()->getImage("spice.png"));
	mSprite->SetPosition(node->getX(), node->getY());
}

Spice::~Spice() {
	if (mSprite) {
		delete mSprite;
		mSprite = 0;
	}
}
