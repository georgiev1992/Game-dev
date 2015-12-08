#include "Shiplist.h"


Shiplist::Shiplist(void) {}

Shiplist::Shiplist(Ogre::SceneManager* newManager) {
	manager = newManager;
	numShips = 0;
	ships = std::vector<Spaceship*>();
}

Shiplist::~Shiplist(void) {
	//destroyShips();
	manager = NULL;
}

Spaceship* Shiplist::operator[](int i) {
	if( i > numShips ) {
		return ships[0];
    }
    return ships[i];
}


void Shiplist::operate(Ogre::Vector3 playerPos,Ogre::Camera *camera, int *hp) {
	// Makes all of the ships run
	// Deletes all ships if each one is dead
	BOOL allDead = true;

	for (int i = 0; i < numShips; ++i) {
		Spaceship* current = ships[i];
		if (current != NULL && !current->isDead()) {
			current->run(playerPos, camera, hp);
			allDead = false;
		}
	}

	if (allDead)
		destroyShips();
}

void Shiplist::addShip(int type, Ogre::SceneNode* newModel) {
	// Add ships to the list according to the type given
	if (numShips < MAX_SHIPS) {
		Spaceship* newShip;


		if (type == 1) {
			newShip = new Smallship(newModel);
		}
		else if (type == 2) {
			newShip = new Medship(newModel);
		}
		else if (type == 3) {
			newShip = new Bigship(newModel);
		}

		ships.push_back(newShip);
		++numShips;
	}
}

void Shiplist::destroyShips() {
	for (int i = 0; i < numShips; ++i) {
		destroy(ships[i]);
	}
	numShips = 0;
}

void Shiplist::destroy(Spaceship* ship) {

	Ogre::SceneNode* temp = ship->getModel();
	temp->removeAndDestroyAllChildren();
	manager->destroySceneNode(temp);
}