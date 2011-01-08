#include "Spice.hpp"
#include "NodeManager.hpp"
#include "AssetManager.hpp"
#include <SFML/Graphics/Sprite.hpp>
using sf::Sprite;

Spice::Spice(Coordinate coor) {
	GameObject::GameObject();
	setNode(NodeManager::getInstance()->getNode(coor));
	mSprite = new Sprite();
	mSprite->SetImage(*AssetManager::getInstance()->getImage("spice.png"));
	mSprite->SetPosition(ConvertXCoordinate(coor.x), ConvertYCoordinate(coor.y));
}

Spice::~Spice() {
	if (mSprite) {
		delete mSprite;
		mSprite = 0;
	}
}
