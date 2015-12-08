#ifndef MEDSHIP_H
#define MEDSHIP_H

#include "Spaceship.h"

class Medship : public Spaceship 
{
public:
	Medship(void) {}

	Medship(Ogre::SceneNode* newModel) {
		// TODO
		// Change these values to be unique to each ship type
		model = newModel;
		health = 20;
		size = 0.8f;
		speed = 0.03f;
		fireSpeed = 1.0f;
	}

	~Medship(void) {}

	virtual void run(Ogre::Vector3 playerPos,Ogre::Camera *camera) {
		move(playerPos, camera);

		// Rotates the four small guns on the front
		model->getChild(0)->getChild(0)->getChild(0)->roll(Ogre::Degree(5));
		model->getChild(0)->getChild(0)->getChild(1)->roll(Ogre::Degree(5));

		model->getChild(1)->getChild(0)->getChild(0)->roll(Ogre::Degree(5));
		model->getChild(1)->getChild(0)->getChild(1)->roll(Ogre::Degree(5));
	}
};

#endif 