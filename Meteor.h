/*
=================
cAsteroid.h
- Header file for class definition - SPECIFICATION
- Header file for the Rocket class which is a child of cSprite class
=================
*/
#ifndef _CMETEOR_H
#define _CMETEOR_H
#include "Sprite.h"

class cMeteor : public cSprite
{
private:
	SDL_Point meteorVelocity;

public:
	cMeteor();
	void update(double deltaTime);		// Rocket update method
	void setMeteorVelocity(SDL_Point asteroidVel);   // Sets the velocity for the asteroid
	SDL_Point getMeteorVelocity();				 // Gets the asteroid velocity
};
#endif