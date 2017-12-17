/*
================
cBkGround.h
- Header file for class definition - SPECIFICATION
- Header file for the Background class which is a child of cSprite class
=================
*/
#ifndef _BACKGROUND_H
#define _BACKGROUND_H
#include "Sprite.h"

class cBkGround : public cSprite
{
public:
	void render();		// Default render function
	void update();		// Update method
};
#endif