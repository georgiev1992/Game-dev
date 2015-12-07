#ifndef SHIPLIST_H
#define SHIPLIST_H

#include <exception>
#include <string>

#include "ogre_application.h"

#include "Smallship.h"
#include "Medship.h"
#include "Bigship.h"

#define MAX_SHIPS 30

class Shiplist
{
public:
	Shiplist(void);
	Shiplist(Ogre::SceneManager* newManager);
	~Shiplist(void);
	void operate(Ogre::Vector3 playerPos);
	void addShip(int type, Ogre::SceneNode* newModel);
	void destroyShips();
	void destroy(Spaceship* ship);

private:
	Ogre::SceneManager* manager;
	std::vector<Spaceship*> ships;
	int numShips;

};

#endif

