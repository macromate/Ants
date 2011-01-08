#pragma once
#ifndef GAME_OBJECT_MANAGER_HPP
#define GAME_OBJECT_MANAGER_HPP

#include "GameObject.hpp"
#include <vector>

class GameObjectManager {
public:
	static GameObjectManager* getInstance();
	virtual ~GameObjectManager();
	GameObject* add(GameObject*);
	void trigger();
	inline std::vector<GameObject*> getObjects() const { return mObjects; }
protected:
	std::vector<GameObject*> mObjects;
private:
	GameObjectManager();
	GameObjectManager(const GameObjectManager&);
	static GameObjectManager* mInstance;
};

#endif
