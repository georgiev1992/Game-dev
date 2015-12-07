#include "Shiplist.h"


Shiplist::Shiplist(void) {}

Shiplist::Shiplist(Ogre::SceneManager* newManager) {
	manager = newManager;
	numShips = 0;
	ships = std::vector<Spaceship*>();
}

Shiplist::~Shiplist(void) {
	destroyShips();
	manager = NULL;
}


void Shiplist::operate(Ogre::Vector3 playerPos) {
	// Makes all of the ships run
	// Deletes all ships if each one is dead
	BOOL allDead = true;

	for (int i = 0; i < numShips; ++i) {
		Spaceship* current = ships[i];
		if (current != NULL && !current->isDead()) {
			current->run(playerPos);
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
	// TODO
	// Needs to be fixed, doesn't work
	Ogre::SceneNode* temp = ship->getModel();
	temp->removeAndDestroyAllChildren();
	manager->destroySceneNode(temp);
}