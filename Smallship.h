#ifndef SMALLSHIP_H
#define SMALLSHIP_H

#include "Spaceship.h"

class Smallship : public Spaceship 
{
public:
	Smallship(void) {}
	Smallship(Ogre::SceneNode* newModel) {
		// TODO
		// Change these values to be unique to each ship type
		model = newModel;
		health = 20;
		size = 0.6f;
		speed = 0.03f;
		fireSpeed = 1.0f;
	}
	~Smallship(void) {}

};

#endif 