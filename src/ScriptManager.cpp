#include "ScriptManager.hpp"

#include "Ant.hpp"

#include <iostream>
#include <stdio.h>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

using namespace boost::python;

BOOST_PYTHON_MODULE(Ants) {
  class_<Coordinate>("Coordinate", init<int, int>())
    .def("get_x", &Coordinate::getX)
    .def("get_y", &Coordinate::getY);
    
  class_<GameObject>("GameObject", no_init)
    .def("get_radius", &GameObject::getRadius)
    .def("set_radius", &GameObject::setRadius)
    .def("get_id", &GameObject::getId);
    
  // class_<Ant>("Ant", no_init)
  //   .def("get_radius", &Ant::getRadius)
  //   .def("set_radius", &Ant::setRadius);
}

ScriptManager *ScriptManager::mInstance = 0;

ScriptManager* ScriptManager::getInstance() {
	if(!mInstance)
		mInstance = new ScriptManager();
	return mInstance;
}


ScriptManager::ScriptManager() {
  Py_Initialize();
  initAnts();
}

ScriptManager::ScriptManager(const ScriptManager& ref) {
}

ScriptManager::~ScriptManager() {
  Py_Finalize();
	delete mInstance;
}

void ScriptManager::startScript(string pName) {
  try {
    FILE* pyFile = fopen((PATH_TO_SCRIPTS + pName + ".py").c_str(), "r");
    PyRun_SimpleFile(pyFile, pName.c_str());

  } catch (error_already_set) {
    PyErr_Print();
  }
}

// TODO: Unregister Gameobjects!!
void ScriptManager::registerGameObject(std::string pName, GameObject* pObject) {
  object main_module(handle<>(borrowed(PyImport_AddModule("__main__"))));
  object main_namespace = main_module.attr("__dict__");
  main_namespace[pName] = ptr(pObject);
}
