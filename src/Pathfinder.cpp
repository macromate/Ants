#include "Pathfinder.hpp"

PathfinderNode::PathfinderNode(Node *node, PathfinderNode *parent, Node *target) :
mNode(node),
mParent(parent),
mGCost(0.0),
mHCost(0.0),
mTarget(target) {
	calculateCosts();
}

PathfinderNode::PathfinderNode(Node *node) :
mNode(node),
mParent(0),
mGCost(0.0),
mHCost(0.0),
mTarget()
{
}

PathfinderNode::~PathfinderNode()
{
}

void PathfinderNode::calculateCosts() {
	float movementCost = 10.0;
	//	if(NodeManager::getInstance()->areNodesDiagonal(node, parent->getNode()))
	//		movementCost = 14.0;
	mGCost = mParent->getGCost() + movementCost;
	mHCost = std::max(abs(mNode->getX() - mTarget->getX()), 
		abs(mNode->getY() - mTarget->getY())) * movementCost;
}

bool PathfinderNode::isEqual(PathfinderNode* node) {
	return getNode()->getX() == node->getNode()->getX() &&
		getNode()->getY() == node->getNode()->getY();
}

PathfinderNode* NodeQueue::top() const {
	return (*mMap.begin()).second;
}

NodeQueue::NodeQueue() :
mMap()
{
}

void NodeQueue::pop() {
	mMap.erase(mMap.begin());
}

bool NodeQueue::includes(PathfinderNode* node) const {
	PathfinderNodeMap::const_iterator i = mMap.begin();
	for (; i != mMap.end(); i++) {
		if (node->isEqual((*i).second))
			return true;
	}
	
	return false;
}

bool NodeQueue::empty() const {
	return mMap.empty();
}

void NodeQueue::push(PathfinderNode* node) {
	mMap.insert(std::pair<float, PathfinderNode*>(node->getFCost(), node));
}

void NodeQueue::update(PathfinderNode* node) {
	std::pair<float, PathfinderNode*> aPair = std::pair<float, PathfinderNode*>(node->getFCost(), node);
	PathfinderNodeMap::iterator position = mMap.begin();
	for (; position != mMap.end(); position++) {
		if((*position).second->isEqual(node)) {
			mMap.erase(position);
			mMap.insert(std::pair<float, PathfinderNode*>(node->getFCost(), node));
			break;
		}
	}
}

Pathfinder::Pathfinder(Node* start, Node* target) :
mStart(start),
mTarget(target),
mOpenList(),
mClosedList()
{
}

Pathfinder::~Pathfinder()
{
	while (!mOpenList.empty()) {
		delete mOpenList.top();
		mOpenList.pop();
	}

	
	PathfinderNodes::iterator i = mClosedList.begin();
	for (; i < mClosedList.end(); i++) {
		delete *i;
	}
}

Path Pathfinder::search() {
	PathfinderNode *actual;
	PathfinderNode *parent;
	
	parent = new PathfinderNode(mStart);
	mOpenList.push(parent);
	
	while(!mOpenList.empty()) {
		actual = mOpenList.top();
		mOpenList.pop();
		mClosedList.push_back(actual);
		if (actual->getNode() == mTarget) {
			break;
		}
//		{
//			std::cout << "Actual X: " << actual->getNode()->getX() << " and Y: " << actual->getNode()->getY() << std::endl;
//			std::cout << "Actual F Cost = " << actual->getFCost() << " G Cost = " << actual->getGCost() << " H Cost = " << actual->getHCost() << std::endl;
//			std::cout << "Closed List Dump:" << std::endl;
//			PathfinderNodes::iterator i = mClosedList.begin();
//			for (; i < mClosedList.end(); i++) {
//				std::cout << "X=" << (*i)->getNode()->getX() << " Y=" << (*i)->getNode()->getY() << std::endl;
//			}
//			std::cout << "------------------------------------------------\n\n" << std::endl;
//		}
		
		addNeighbourNodesToOpenList(actual);
	}
	
	Path path;
	
	if (mTarget) {
		PathfinderNodes::const_iterator i = mClosedList.begin();
		for(; i < mClosedList.end(); i++) {
			path.push_back((*i)->getNode());
		}
        path.erase(path.begin()); // delete start node
		return path;
	} else {
		return path;
	}
}

void Pathfinder::addNeighbourNodesToOpenList(PathfinderNode* parent) {
	Nodes neighbours = NodeManager::getInstance()->getNeighbourNodes(parent->getNode());
	Nodes::iterator i = neighbours.begin();
	for (; i < neighbours.end(); i++) {
		Node* actual = *i;
		if (actual->isAccessible()) {
			PathfinderNode* pf = new PathfinderNode(actual, parent, mTarget);
			if (mOpenList.includes(pf)) {
				//std::cout << "Already in open list " << pf->getNode()->getX() << "/" << pf->getNode()->getY() << std::endl;
				if (pf->getGCost() < parent->getGCost()) {
					std::cout << pf->getGCost() << " < " << parent->getGCost() << std::endl;
					parent->setParent(pf);
					mOpenList.update(parent);
				}
			} else {
				//std::cout << "Not in open list " << pf->getNode()->getX() << "/" << pf->getNode()->getY() << std::endl;
				mOpenList.push(pf);
			}
		}
	}
}
