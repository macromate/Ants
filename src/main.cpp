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
#include "Sprite.hpp"
#include "Hill.hpp"

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
				node = new Node(x * 32, y * 32);
				nodeManager->addNode(node);
			}
		}
	}
	
    Hill* hill1 = new Hill(StrainYellow, nodeManager->getEmptyRandomNode());
    Hill* hill2 = new Hill(StrainRed, nodeManager->getEmptyRandomNode());
    Hill* hill3 = new Hill(StrainBlue, nodeManager->getEmptyRandomNode());
    objectManager->add(hill1);
    objectManager->add(hill2);
    objectManager->add(hill3);
	
    Ants ants;
    Strain antStrains[4] = { StrainYellow, StrainRed, StrainBlue, StrainRed };
    for (int i = 0; i < 4; i++) {
        Ant* ant = new Ant(antStrains[i], nodeManager->getEmptyRandomNode());
        objectManager->add(ant);
        ants.push_back(ant);
    }

    for (int i = 0; i < 4; i++) {
        Spice *spice = new Spice(nodeManager->getEmptyRandomNode());
        objectManager->addSpice(spice);
    }
    
    sf::RenderWindow App(sf::VideoMode(640, 480), "Ants");
    while (App.IsOpened())
    {
        sf::Event Event;
        while (App.GetEvent(Event))
        {
            if (Event.Type == sf::Event::Closed)
                App.Close();
            if (Event.Type == sf::Event::MouseButtonPressed) {
                const sf::Input& Input = App.GetInput();
                cout << "Mouse cursor position: " << Input.GetMouseX() 
                     <<  "/" << Input.GetMouseY() << endl;
            }
        }
		
        App.Clear();
		
		// trigger updates
        objectManager->trigger();

        //display nodes
        Nodes nodes = nodeManager->getNodes();
        Nodes::iterator n = nodes.begin();
        for (; n < nodes.end(); n++) {
			App.Draw(*(*n)->getSprite());
        }
		
		// update ants
        Ants::iterator a = ants.begin();
        for(; a < ants.end(); a++) {
            (*a)->handleCurrentAction(App.GetFrameTime());
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
