#include "Smallship.h"


Smallship::Smallship(void)
{
}

Smallship::Smallship(Ogre::SceneNode* newModel) {
	// TODO
	// Change these values to be unique to each ship type
	model = newModel;
	health = 20;
	speed = 0.05f;
	fireSpeed = 1.0f;
}

Smallship::~Smallship(void)
{
}
