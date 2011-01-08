#pragma once
#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#define PATH_TO_ASSETS "assets/"

#include "Definitions.hpp"
#include <SFML/Graphics/Image.hpp>
#include <map>
#include <string>

class AssetManager {
public:
	static AssetManager* getInstance();
	sf::Image* getImage(std::string name);
	~AssetManager();
private:
	AssetManager();
	AssetManager(const AssetManager&);
	static AssetManager* mInstance;
	std::map<std::string, sf::Image*> mImages;
};

#endif
