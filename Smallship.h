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

	virtual void run(Ogre::Vector3 playerPos,Ogre::Camera *camera,int* hp) {
		move(playerPos, camera, hp);

		// Rotates the torus
		Ogre::Vector3 s1, s2;
		s1 = model->getChild(0)->getScale();
		s2 = model->getChild(1)->getScale();

		if (s1.x == s2.x && s1.y == s2.y && s1.z == s2.z)
			model->getChild(2)->roll(Ogre::Degree(5));
		else
			model->getChild(1)->roll(Ogre::Degree(5));
	}

};

#endif 