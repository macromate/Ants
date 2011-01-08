#include "AssetManager.hpp"

using std::string;
using std::map;
using std::pair;

AssetManager* AssetManager::mInstance = 0;

AssetManager* AssetManager::getInstance() {
	if (!mInstance)
		mInstance = new AssetManager();
	return mInstance;
}

AssetManager::~AssetManager() {
	
}

AssetManager::AssetManager() : 
mImages() {
	map<string, sf::Image*>::iterator i = mImages.begin();
	for (; i != mImages.end(); i++) {
		delete (*i).second;
	}
}

sf::Image* AssetManager::getImage(std::string name) {
	if (mImages[name]) {
		return mImages[name];
	}
	
	sf::Image* image = new sf::Image;
	image->LoadFromFile(PATH_TO_IMAGES + name);
	
	mImages.insert(pair<string, sf::Image*>(name, image));
	return image;
}


