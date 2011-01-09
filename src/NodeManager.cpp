#include "NodeManager.hpp"

NodeManager *NodeManager::mInstance = 0;

using std::vector;

NodeManager* NodeManager::getInstance() {
	if(!mInstance)
		mInstance = new NodeManager;
	return mInstance;
}

NodeManager::NodeManager() :
mNodes(0), mLeftBorder(0), mRightBorder(0), mTopBorder(0), mBottomBorder(0)
{
	std::cout << "nodemanager created" << std::endl;
}

NodeManager::~NodeManager() {
	std::cout << "nodemanager deleted" << std::endl;
	Nodes::iterator i = mNodes.begin();
	for (; i < mNodes.end(); i++)
		delete (*i);
}

void NodeManager::addNode(Node* node) {
	mNodes.push_back(node);
	if(node->getX() < mLeftBorder) mLeftBorder = node->getX();
	if(node->getY() < mBottomBorder) mBottomBorder = node->getY();
	if(node->getX() > mRightBorder) mRightBorder = node->getX();
	if(node->getY() > mTopBorder) mTopBorder = node->getY();
}

Node* NodeManager::getNode(int x, int y) {
	Nodes::iterator i = mNodes.begin();
	for (; i < mNodes.end(); i++)
		if ((*i)->getX() == x && (*i)->getY() == y)
			return *i;
	return 0;
}

Node* NodeManager::getNode(Coordinate coor) {
	return getNode(coor.getX(), coor.getY());
}

Nodes NodeManager::getNeighbourNodes(Node* node) {
	Nodes neighbours = Nodes();
	Coordinate coor = Coordinate(node->getX() - 1, node->getY() + 1);
	if(isIncluded(coor)) neighbours.push_back(getNode(coor));
	coor = Coordinate(node->getX(), node->getY() + 1);
	if(isIncluded(coor)) neighbours.push_back(getNode(coor));
	coor = Coordinate(node->getX() + 1, node->getY() + 1);
	if(isIncluded(coor)) neighbours.push_back(getNode(coor));
	coor = Coordinate(node->getX() - 1, node->getY());
	if(isIncluded(coor)) neighbours.push_back(getNode(coor));
	coor = Coordinate(node->getX() + 1, node->getY());
	if(isIncluded(coor)) neighbours.push_back(getNode(coor));
	coor = Coordinate(node->getX() - 1, node->getY() - 1);
	if(isIncluded(coor)) neighbours.push_back(getNode(coor));
	coor = Coordinate(node->getX(), node->getY() - 1);
	if(isIncluded(coor)) neighbours.push_back(getNode(coor));
	coor = Coordinate(node->getX() + 1, node->getY() - 1);
	if(isIncluded(coor)) neighbours.push_back(getNode(coor));
	
	return neighbours;
}

bool NodeManager::isIncluded(Coordinate coor) {
	return coor.getX() >= mLeftBorder && coor.getY() >= mBottomBorder &&
		   coor.getX() <= mRightBorder && coor.getY() <= mTopBorder;
}

bool NodeManager::areNodesDiagonal(Node* n1, Node* n2) {
	return (n1->getX() - 1 == n2->getX() && n1->getY() + 1 == n2->getY()) ||
		   (n1->getX() + 1 == n2->getX() && n1->getY() + 1 == n2->getY()) ||
	       (n1->getX() - 1 == n2->getX() && n1->getY() - 1 == n2->getY()) ||
		   (n1->getX() + 1 == n2->getX() && n1->getY() - 1 == n2->getY());
}

Nodes NodeManager::getNodesInRadius(Node* node, int radius) {
	if (radius == 1)
		return getNeighbourNodes(node);
	
	
	// TODO: Use a C Array here??
	int x = node->getX();
	int y = node->getY();
	vector<Coordinate> coordinates;
	for (int n = 1; n <= radius; n++) {
		coordinates.push_back(Coordinate(x - n, y + n));
		coordinates.push_back(Coordinate(x, y + n));
		coordinates.push_back(Coordinate(x + n, y + n));
		coordinates.push_back(Coordinate(x - n, y));
		coordinates.push_back(Coordinate(x + n, y));
		coordinates.push_back(Coordinate(x - n, y - n));
		coordinates.push_back(Coordinate(x, y - n));
		coordinates.push_back(Coordinate(x + n, y - n));
	}
	
	Nodes nodes;
	vector<Coordinate>::iterator i = coordinates.begin();
	for (; i < coordinates.end(); i++) {
		if (isIncluded(*i)) {
			nodes.push_back(getNode(*i));
		}
	}
	return nodes;
}
