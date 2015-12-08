#include "Bullet.h"

Bullet::Bullet(){
	InUse = 0;
	alive = 0;
	hit = false;
}
Bullet::~Bullet(){
	//todo: destruct when done with
}

void Bullet::setPositionAway(){
		leftBullet->setPosition(          leftBullet->getPosition() + (300000)*traj                  );
		rightBullet->setPosition(         rightBullet->getPosition() + (300000)*traj                  );
		InUse =0;
}

void Bullet::handle(float time){
	if(alive > .5 || InUse == 0){
		alive = 0;
		InUse = 0;
		leftBullet->setVisible(false);
		rightBullet->setVisible(false);
	}else if (InUse == 1){
		leftBullet->setVisible(true);
		rightBullet->setVisible(true);
		alive+=time;

		leftBullet->setPosition(          leftBullet->getPosition() + (30*time)*traj                  );
		rightBullet->setPosition(         rightBullet->getPosition() + (30*time)*traj                  );

	}
	//todo: make collision with bullets and other objects


	
}