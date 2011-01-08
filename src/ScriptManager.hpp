#pragma once
#ifndef SCRIPT_MANAGER_HPP
#define SCRIPT_MANAGER_HPP

#include "Definitions.hpp"

#include <string>

class ScriptManager {
public:
	static ScriptManager *getInstance();
	~ScriptManager();
	void startScript(const char* name);
private:
	ScriptManager();
	ScriptManager(const ScriptManager&);
	static ScriptManager *mInstance;
};

#endif
