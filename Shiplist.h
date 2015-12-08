#ifndef SHIPLIST_H
#define SHIPLIST_H

#include <exception>
#include <string>

#include "ogre_application.h"

#include "Smallship.h"
#include "Medship.h"
#include "Bigship.h"

#define MAX_SHIPS 60

class Shiplist
{
public:
	Shiplist(void);
	Shiplist(Ogre::SceneManager* newManager);
	~Shiplist(void);
	Spaceship* operator[](int i);
	void operate(Ogre::Vector3 playerPos,Ogre::Camera *camera,int *hp);
	void addShip(int type, Ogre::SceneNode* newModel);
	void destroyShips();
	void destroy(Spaceship* ship);
	int getNumShips() {return numShips;};

private:
	Ogre::SceneManager* manager;
	std::vector<Spaceship*> ships;
	int numShips;

};

#endif

