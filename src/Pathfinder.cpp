#include "Pathfinder.hpp"

PathfinderNode::PathfinderNode(Node *pNode, PathfinderNodePtr parent, Node *target) :
mNode(pNode),
mParent(parent),
mGCost(0.0),
mHCost(0.0),
mTarget(target) {
	calculateCosts();
}

PathfinderNode::PathfinderNode(Node *pNode) :
mNode(pNode),
mParent(),
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
	mGCost = mParent->getGCost() + movementCost;
	mHCost = std::max(abs(mNode->getX() - mTarget->getX()), 
		abs(mNode->getY() - mTarget->getY())) * movementCost;
}

bool PathfinderNode::isEqual(PathfinderNodePtr pNode) {
	return getNode()->getX() == pNode->getNode()->getX() &&
		getNode()->getY() == pNode->getNode()->getY();
}

PathfinderNodePtr NodeQueue::top() const {
	return (*mMap.begin()).second;
}

NodeQueue::NodeQueue() :
mMap()
{
}

void NodeQueue::pop() {
	mMap.erase(mMap.begin());
}

bool NodeQueue::includes(PathfinderNodePtr pNode) const {
	PathfinderNodeMap::const_iterator i = mMap.begin();
	for (; i != mMap.end(); i++) {
		if (pNode->isEqual((*i).second))
			return true;
	}
	
	return false;
}

bool NodeQueue::empty() const {
	return mMap.empty();
}

void NodeQueue::push(PathfinderNodePtr pNode) {
	mMap.insert(std::pair<float, PathfinderNodePtr>(pNode->getFCost(), pNode));
}

void NodeQueue::update(PathfinderNodePtr pNode) {
	std::pair<float, PathfinderNodePtr> aPair(pNode->getFCost(), pNode);
	PathfinderNodeMap::iterator position = mMap.begin();
	for (; position != mMap.end(); position++) {
		if((*position).second->isEqual(pNode)) {
			mMap.erase(position);
			mMap.insert(std::pair<float, PathfinderNodePtr>(pNode->getFCost(), pNode));
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
}

Path Pathfinder::search() {
	PathfinderNodePtr actual;
	PathfinderNodePtr parent(new PathfinderNode(mStart));
	Path path;
	
	mOpenList.push(parent);
	
	while(!mOpenList.empty()) {
		actual = mOpenList.top();
		mOpenList.pop();
		mClosedList.push_back(actual);
		
		if (actual->getNode() == mTarget) {
			// path to target is found, abort
			PathfinderNodes::iterator i = mClosedList.begin();
			for(; i < mClosedList.end(); i++) {
				path.push_back((*i)->getNode());
			}
			path.erase(path.begin()); // delete start node
			break;
		}

		// add neighbours to openlist
		Nodes neighbours = NodeManager::getInstance()->getNeighbourNodes(actual->getNode());
		Nodes::iterator i = neighbours.begin();
		for (; i < neighbours.end(); i++) {
			Node* neighbour = *i;
			if (neighbour->isAccessible()) {
				PathfinderNodePtr pf(new PathfinderNode(neighbour, actual, mTarget));
				if (mOpenList.includes(pf)) {
					// already in open list, check costs
					if (pf->getGCost() < actual->getGCost()) {
						actual->setParent(pf);
						mOpenList.update(actual);
					}
				} else {
					// not in open list, add
					mOpenList.push(pf);
				}
			}
		}
	}
	
	return path;
}
