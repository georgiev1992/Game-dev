#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <exception>
#include <string>
#include <time.h>

#include "OGRE/OgreRoot.h"
#include "OGRE/OgreViewport.h"
#include "OGRE/OgreCamera.h"
#include "OGRE/OgreHardwarePixelBuffer.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreCompositorManager.h"
#include "OGRE/OgreCompositorInstance.h"
#include "OIS/OIS.h"

#define PATH_SIZE 5
// TODO
// Add shooting

class Spaceship
{
public:
	Spaceship(void);
	~Spaceship(void);
	void run();
	void move();
	void takeDamage();
	bool isDead();
	Ogre::Vector3 getPosition();
	Ogre::SceneNode* getModel();

protected:
	void moveTo(Ogre::Vector3 newPos);
	void nextPathPos();
	void calcNewPath();
	bool isColliding(Ogre::Vector3 pos);
	void setPosition(Ogre::Vector3 newPos);

	Ogre::SceneNode* model;
	Ogre::Vector3 fPath[PATH_SIZE];
	int health;
	int pathCount;
	float speed;
	float fireSpeed;
	float lastHit;
	//Weapon weapon

};

#endif
