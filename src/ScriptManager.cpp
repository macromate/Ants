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
    
  class_<Ant>("Ant", no_init)
    .def("get_radius", &Ant::getRadius)
    .def("set_radius", &Ant::setRadius);
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

void ScriptManager::startScript(string name) {
  try {
    // set up variables
    object main_module((handle<>(borrowed(PyImport_AddModule("__main__")))));
    object main_namespace = main_module.attr("__dict__");

    // Coordinate coor = Coordinate(4, 14);
    // main_namespace["coor"] = ptr(&coor);
    
    FILE* pyFile = fopen((PATH_TO_SCRIPTS + name + ".py").c_str(), "r");
    PyRun_SimpleFile(pyFile, name.c_str());

  } catch (error_already_set) {
    PyErr_Print();
  }
  
}
