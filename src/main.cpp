#include "Definitions.hpp"
#include "Event.hpp"
#include "EventManager.hpp"
#include "GameObject.hpp"
#include "GameObjectManager.hpp"
#include "Ant.hpp"
#include "NodeManager.hpp"
#include "Pathfinder.hpp"
#include "Signal.hpp"
#include "Spice.hpp"

#include <SFML/Graphics.hpp>

#include <vector>
#include <iostream>

using std::cout;
using std::endl;

// aus irgendeinem grund stürzt gcc ab
// wenn das in die Definitions.hpp geht
// die fitnesspunkte für jeden stamm
// zb: gPoints[StrainYellow] += 10;
int gPoints[3] = { 0, 0, 0 };

int main (int argc, char * const argv[]) {
	GameObjectManager* objectManager = GameObjectManager::getInstance();	
	NodeManager *nodeManager = NodeManager::getInstance();
	{
		
		Node *node;
		for (int y = 0; y <= 14; y++) {
			for (int x = 0; x <= 19; x++) {
				node = new Node(x, y);
				nodeManager->addNode(node);
			}
		}
	}

//	Path path;
//	Pathfinder *p = new Pathfinder(nodeManager->getNode(1, 3), nodeManager->getNode(24, 25));
//	path = p->search();
//	Path::iterator i = path.begin();
//	for (; i < path.end(); i++) {
//		std::cout << (*i).x << "/" << (*i).y << std::endl;
//	}
	//delete p;
	
	Ant *ant1 = new Ant(StrainYellow, Coordinate(4, 6));
	Ant *ant2 = new Ant(StrainRed, Coordinate(8, 6));
	objectManager->add(ant1);
	objectManager->add(ant2);
	Spice *spice = new Spice(Coordinate(5, 7));
	objectManager->add(spice);
	
	sf::Sprite grassSprite;
	grassSprite.SetImage(*AssetManager::getInstance()->getImage("grass.png"));
	
    sf::RenderWindow App(sf::VideoMode(640, 480), "Ants");
    while (App.IsOpened())
    {
        sf::Event Event;
        while (App.GetEvent(Event))
        {
            if (Event.Type == sf::Event::Closed)
                App.Close();
        }
		
        App.Clear();
		
        objectManager->trigger();
		
		// display nodes
		Nodes nodes = nodeManager->getNodes();
		Nodes::iterator n = nodes.begin();
		for (; n < nodes.end(); n++) {
			Node* node = *n;
			int x = ConvertXCoordinate(node->getX());
			int y = ConvertYCoordinate(node->getY()) - 1;
			grassSprite.SetPosition(x, y);
			App.Draw(grassSprite);
		}
		
		// display game objects
		GameObjects objects = objectManager->getObjects();
		GameObjects::iterator i = objects.begin();
		for (; i < objects.end(); i++) {
			GameObject* object = *i;
			if (object->hasSprite()) {
				App.Draw(*object->getSprite());
			}
		}
		
        App.Display();
    }
	
	
	
    return EXIT_SUCCESS;
}
