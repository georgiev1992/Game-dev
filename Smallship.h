#ifndef SMALLSHIP_H
#define SMALLSHIP_H

#include "spaceship.h"

class Smallship : public Spaceship 
{
public:
	Smallship(void);
	Smallship(Ogre::SceneNode* newModel);
	~Smallship(void);

};

#endif 