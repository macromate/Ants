#include "GameObjectManager.hpp"

using std::vector;

GameObjectManager* GameObjectManager::mInstance = 0;

GameObjectManager* GameObjectManager::getInstance() {
	if (!mInstance)
		mInstance = new GameObjectManager();
	return mInstance;
}

GameObjectManager::GameObjectManager() :
mObjects(),
mSpice() {
	
}

GameObjectManager::~GameObjectManager() {
    GameObjects::iterator i = mObjects.begin();
	for (; i < mObjects.end(); i++)
		delete *i;
}

void GameObjectManager::add(GameObject* obj) {
	mObjects.push_back(obj);
}

void GameObjectManager::addSpice(Spice* obj) {
    mObjects.push_back(obj);
    mSpice.push_back(obj);
}

void GameObjectManager::trigger() {
	GameObjects::iterator i = mObjects.begin();
	for (; i < mObjects.end(); i++)
		(*i)->trigger();
}

void GameObjectManager::deleteSpice(Spice* obj) {
    mObjects.erase(std::find(mObjects.begin(), mObjects.end(), obj));
    mSpice.erase(std::find(mSpice.begin(), mSpice.end(), obj));
    delete obj;
}
