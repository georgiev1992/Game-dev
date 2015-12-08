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

#define PATH_SIZE 10


class Spaceship
{
public:
	Spaceship(void);
	Spaceship(Ogre::SceneNode* newModel);
	~Spaceship(void);
	virtual void run(Ogre::Vector3 playerPos,Ogre::Camera *camera,int *hp);
	void move(Ogre::Vector3 playerPos, Ogre::Camera *camera,int *hp);
	void takeDamage(int i);
	bool isDead();
	float getSize() {return size;};
	Ogre::Vector3 getPosition();
	Ogre::SceneNode* getModel();

protected:
	void moveTo(Ogre::Vector3 newPos);
	void nextPathPos();
	void calcNewPath();
	void setupDirection();
	bool isColliding(Ogre::Vector3 pos);
	void setPosition(Ogre::Vector3 newPos);

	Ogre::SceneNode* model;
	Ogre::Vector3 fPath[PATH_SIZE];
	int health;
	int pathCount;
	float size;
	float speed;
	float fireSpeed;
	float lastHit;
	//Weapon weapon

};

#endif
