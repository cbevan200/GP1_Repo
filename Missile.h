/*
=================
cBullet.h
- Header file for class definition - SPECIFICATION
- Header file for the Bullet class which is a child of cSprite class
=================
*/
#ifndef _MISSILE_H
#define _MISSILE_H
#include "Sprite.h"


class cMissile : public cSprite
{
private:
	SDL_Point missileVelocity;

public:
	cMissile();
	void update(double deltaTime);		// Bullet update method
	void setMissileVelocity(SDL_Point bulletVel);   // Sets the velocity for the missile
	SDL_Point getMissileVelocity();				 // Gets the missile velocity
};
#endif