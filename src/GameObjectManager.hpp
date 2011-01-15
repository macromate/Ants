#pragma once
#ifndef GAME_OBJECT_MANAGER_HPP
#define GAME_OBJECT_MANAGER_HPP

#include "Definitions.hpp"
#include "GameObject.hpp"
#include "Spice.hpp"
#include <vector>
#include <algorithm>

class GameObjectManager {
public:
	static GameObjectManager* getInstance();
	virtual ~GameObjectManager();
	void add(GameObject*);
    void addSpice(Spice*);
    void deleteSpice(Spice*);
	void trigger();
	inline GameObjects getObjects() const { return mObjects; }
    inline std::vector<Spice*> getSpice() const { return mSpice; }
protected:
	GameObjects mObjects;
    std::vector<Spice*> mSpice;
private:
	GameObjectManager();
	GameObjectManager(const GameObjectManager&);
	static GameObjectManager* mInstance;
};

#endif
