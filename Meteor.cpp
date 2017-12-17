/*
=================
cAsteroid.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "Meteor.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cMeteor::cMeteor() : cSprite()
{
	this->meteorVelocity = { 0, 0 };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cMeteor::update(double deltaTime)
{

	this->setSpriteRotAngle(this->getSpriteRotAngle() +(5.0f * deltaTime)); 
	if (this->getSpriteRotAngle() > 360)
	{
		this->setSpriteRotAngle(this->getSpriteRotAngle() -360);
	}

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += this->getSpriteTranslation().x * deltaTime;
	currentSpritePos.y += this->getSpriteTranslation().y * deltaTime;

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
	//cout << "Asteroid position - x: " << this->getSpritePos().x << " y: " << this->getSpritePos().y << " deltaTime: " << deltaTime << endl;
	this->setBoundingRect(this->getSpritePos());
}
/*
=================================================================
  Sets the velocity for the Meteor
=================================================================
*/
void cMeteor::setMeteorVelocity(SDL_Point MeteorVel)
{
	meteorVelocity = MeteorVel;
}
/*
=================================================================
  Gets the Meteor velocity
=================================================================
*/
SDL_Point cMeteor::getMeteorVelocity()
{
	return meteorVelocity;
}
