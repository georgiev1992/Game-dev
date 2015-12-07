#ifndef BULLET_H
#define BULLET_H

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


// TODO
// Add shooting

class Bullet
{
public:
	Bullet();
	~Bullet();
	void handle(float);
	void setPositionAway();
	//void 
public:
	int InUse;
	float alive;
	bool hit;
	Ogre::SceneNode* leftBullet;
	Ogre::SceneNode* rightBullet;
	Ogre::Vector3 traj;
	Ogre::SceneNode* parentShip;


};

#endif