#include "ScriptManager.hpp"
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::string;

ScriptManager *ScriptManager::mInstance = 0;

ScriptManager* ScriptManager::getInstance() {
	if(!mInstance)
		mInstance = new ScriptManager();
	return mInstance;
}


ScriptManager::ScriptManager() {
}

ScriptManager::ScriptManager(const ScriptManager& ref) {
}

ScriptManager::~ScriptManager() {
	delete mInstance;
}

void ScriptManager::startScript(const char* name) {
  cout << "Should be executing " << PATH_TO_SCRIPTS <<  name << ".py" << endl; 
}
