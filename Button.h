/*
=================
cButton.h
- Header file for class definition - SPECIFICATION
- Header file for the tileMap class which is a child of cSprite class
=================
*/
#ifndef _BUTTON_H
#define _BUTTON_H
#include "Sprite.h"

class cButton : public cSprite
{

private:

	SDL_Point buttonClickedRC;
	bool clicked = false;

public:
	cButton();

	gameState update(gameState theCurrentGameState, gameState newGameState, SDL_Point theAreaClicked);

	bool getClicked();
	void setClicked(bool state);

};
#endif