#pragma once
#ifndef SCRIPT_MANAGER_HPP
#define SCRIPT_MANAGER_HPP

#include "Definitions.hpp"

#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>

#include <string>

class ScriptManager {
public:
	static ScriptManager *getInstance();
	~ScriptManager();
  void registerGameObject(std::string pName, GameObject* pObject);
	void startScript(std::string pName);
private:
	ScriptManager();
	ScriptManager(const ScriptManager&);
	static ScriptManager *mInstance;
};

#endif
