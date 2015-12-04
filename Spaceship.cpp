#include "Spaceship.h"

Spaceship::Spaceship(void)
{
	model = NULL;
	health = 20;
	speed = 0.1f;
	fireSpeed = 1.0f;
	lastHit = 0.0f;
	calcNewPath();
}

Spaceship::~Spaceship(void)
{
	// TODO
	// Get list class to destroy the model
}

void Spaceship::run() {
	// TODO
	// Call all functions to make the ship operate (e.g. moving and shooting)
	move();
}

void Spaceship::move() {
	// Move ship to its next position
	// TODO
	// Check for collision with player and other ships
	if (pathCount < PATH_SIZE) {
		if (isColliding(fPath[PATH_SIZE])) {
			calcNewPath(); 
		}
		if (isColliding(fPath[pathCount])) {
			nextPathPos();
		}

		moveTo(fPath[pathCount]);
	} else {
		calcNewPath();
		move();
	}
}

void Spaceship::moveTo(Ogre::Vector3 newPos) {
	// Move by a small amount toward the newPos
	Ogre::Vector3 direction = newPos - getPosition();
	direction.normalise();
	model->setPosition(getPosition() + direction*speed);
}

void Spaceship::nextPathPos() { 
	// Changes pathCount and rotates model to this direction
	Ogre::Vector3 dir1, dir2;
	dir1 = fPath[pathCount] - getPosition();
	dir1.y = 0;
	dir1.normalise();
	

	++pathCount;
	if (pathCount == PATH_SIZE)
		calcNewPath();

	dir2 = fPath[pathCount] - getPosition();
	dir2.y = 0;
	dir2.normalise();
	
	Ogre::Radian angle = dir1.angleBetween(dir2);
	model->yaw(-angle);
	
}

void Spaceship::calcNewPath() {
	// TODO
	// Change to use splines to calculate path
	Ogre::Vector3 pos = getPosition();
	int x, y, z;
	for (int i = 0; i < PATH_SIZE; ++i) {
		x = pos.x + rand() % 20 - 10;
		y = pos.y + rand() % 20 - 10;
		z = pos.z + rand() % 20 - 10;
		fPath[i] = Ogre::Vector3(x, y, z);
	}
	pathCount = 0;
}

void Spaceship::takeDamage() {
	// TODO
	// Take damage equal to what the ship is being hit by
	// i.e. colliding and weapon damage
	time_t seconds = time(0);
	if (seconds - lastHit > 1) {
		lastHit = seconds;
	}
}

bool Spaceship::isDead() {
	if (health < 1)
		return true;
	return false;
}

bool Spaceship::isColliding(Ogre::Vector3 pos) {
	if ((getPosition().distance(pos)) < 1)
		return true;
	return false;
}

void Spaceship::setPosition(Ogre::Vector3 newPos) {
	model->setPosition(newPos);
}

Ogre::Vector3 Spaceship::getPosition() {
	if (model)
		return model->getPosition();
	return Ogre::Vector3(0.0f,0.0f,0.0f);
}

Ogre::SceneNode* Spaceship::getModel() {
	return model;
}
