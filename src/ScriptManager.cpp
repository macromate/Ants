#include "ScriptManager.hpp"

#include "Ant.hpp"
#include "Node.hpp"
#include "NodeManager.hpp"

#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

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
        .def("get_id", &GameObject::getId)
        .def("get_node", &GameObject::getNode, return_value_policy<reference_existing_object>());

    class_<Node, boost::noncopyable>("Node", no_init)
        .def("get_coordinate", &Node::getCoordinate, return_value_policy<return_by_value>())
        .def("get_x", &Node::getX)
        .def("get_y", &Node::getY);

    Node* (NodeManager::*getNode1)(int, int) const = &NodeManager::getNode;
    Node* (NodeManager::*getNode2)(Coordinate) const = &NodeManager::getNode;

    class_<NodeManager, boost::noncopyable>("NodeManager", no_init)
        .def("get_neighbour_nodes", &NodeManager::getNeighbourNodes)
        .def("get_node", getNode1, return_value_policy<reference_existing_object>())
        .def("get_node", getNode2, return_value_policy<reference_existing_object>())
        .def("get_random_node", &NodeManager::getRandomNode, return_value_policy<reference_existing_object>())
        .def("get_empty_random_node", &NodeManager::getEmptyRandomNode, return_value_policy<reference_existing_object>());

    class_<Nodes>("Nodes")
        .def(vector_indexing_suite<Nodes>());

    class_<Ant, bases<GameObject> >("Ant", no_init)
        .def("set_move_target", &Ant::setMoveTarget)
        .def("get_strain", &Ant::getStrain);

    class_<Spice, bases<GameObject> >("Spice", init<Node*>());

    enum_<Strain>("Strain")
        .value("yellow", StrainYellow)
        .value("blue", StrainBlue)
        .value("red", StrainRed);
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
    
    // register mainNamespace
    object main_module = import("__main__");
    mMainNamespace = main_module.attr("__dict__");
    // register nodeManager
    mMainNamespace["nodeManager"] = ptr(NodeManager::getInstance());
}

ScriptManager::ScriptManager(const ScriptManager& ref) {
}

ScriptManager::~ScriptManager() {
    Py_Finalize();
    delete mInstance;
}

void ScriptManager::startScript(string pName) {
    try {
        object main_module = import("__main__");
        object main_namespace = main_module.attr("__dict__");

        pName = PATH_TO_SCRIPTS + pName + ".py";
        exec_file(str(pName), main_namespace);

    } catch (error_already_set) {
        PyErr_Print();
    }
}

// TODO: Unregister Gameobjects!!
void ScriptManager::registerGameObject(std::string pName, GameObject* pObject) {
    mMainNamespace[pName] = ptr(pObject);
}
