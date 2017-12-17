/*
=================
cBullet.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "Missile.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cMissile::cMissile() : cSprite()
{
	this->missileVelocity = { 0, 0 };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cMissile::update(double deltaTime)
{

	FPoint direction = { 0.0, 0.0};
	direction.X = (sin((this->getSpriteRotAngle())*PI/180));
	direction.Y = -(cos((this->getSpriteRotAngle())*PI / 180));

	direction.X *= this->getSpriteTranslation().x;
	direction.Y *= this->getSpriteTranslation().y;

	this->missileVelocity.x = this->missileVelocity.x + direction.X;
	this->missileVelocity.y = this->missileVelocity.y + direction.Y;

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += this->missileVelocity.x * deltaTime;
	currentSpritePos.y += this->missileVelocity.y * deltaTime;

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });

	cout << "Bullet position - x: " << this->getSpritePos().x << " y: " << this->getSpritePos().y << " deltaTime: " << deltaTime << endl;

	this->setBoundingRect(this->getSpritePos());

}
/*
=================================================================
  Sets the velocity for the Bullet
=================================================================
*/
void cMissile::setMissileVelocity(SDL_Point bulletVel)
{
	this->missileVelocity = bulletVel;
}
/*
=================================================================
  Gets the Bullet velocity
=================================================================
*/
SDL_Point cMissile::getMissileVelocity()
{
	return this->missileVelocity;
}
