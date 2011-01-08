#include "GameObjectManager.hpp"

using std::vector;

GameObjectManager* GameObjectManager::mInstance = 0;

GameObjectManager* GameObjectManager::getInstance() {
	if (!mInstance)
		mInstance = new GameObjectManager();
	return mInstance;
}

GameObjectManager::GameObjectManager() :
mObjects() {
	
}

GameObjectManager::~GameObjectManager() {
	vector<GameObject*>::iterator i = mObjects.begin();
	for (; i < mObjects.end(); i++)
		delete *i;
}

GameObject* GameObjectManager::add(GameObject* obj) {
	mObjects.push_back(obj);
	return obj;
}

void GameObjectManager::trigger() {
	vector<GameObject*>::iterator i = mObjects.begin();
	for (; i < mObjects.end(); i++)
		(*i)->trigger();
}
