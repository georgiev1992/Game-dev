#ifndef BIGSHIP_H
#define BIGSHIP_H

#include "Spaceship.h"

class Bigship : public Spaceship 
{
public:
	Bigship(void) {}

	Bigship(Ogre::SceneNode* newModel) {
		// TODO
		// Change these values to be unique to each ship type
		model = newModel;
		health = 20;
		size = 1.0f;
		speed = 0.03f;
		fireSpeed = 1.0f;
	}

	~Bigship(void) {}

	virtual void run(Ogre::Vector3 playerPos,Ogre::Camera *camera) {
		move(playerPos, camera);

		// Rotates guns on the left side
		model->getChild(0)->getChild(0)->getChild(0)->roll(Ogre::Degree(5));
		model->getChild(0)->getChild(1)->getChild(0)->roll(Ogre::Degree(5));

	}

};

#endif 