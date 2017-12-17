#include "Game.h"
#include "ButtonMgr.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}

void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);

	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();

	// Store the textures
	textureName = { "asteroid1", "asteroid2", "asteroid3", "asteroid4", "bullet", "theRocket", "theBackground", "menuBackground", "endingBackground"};
	texturesToUse = { "Images\\Meteor1.png", "Images\\Meteor2.png", "Images\\Meteor3.png", "Images\\Meteor4.png", "Images\\Missile.png", "Images\\Rocket.png", "Images\\Earth.png", "Images\\Spacebackground.jpg", "Images\\EndSceneBackground.jpg" };
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}
	btnNameList = { "exit_btn", "menu_btn", "play_btn" };
	btnTexturesToUse = { "Images/Buttons/Exit_Button.png", "Images/Buttons/Menu_Button.png", "Images/Buttons/Play_Button.png" };
	btnPos = { { 400, 375 },{ 400, 300 },{ 400, 300 },{ 400, 375 }};
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}

	theRocket.setTexture(theTextureMgr->getTexture("theRocket"));
	theRocket.setSpritePos({ 500, 350 });

	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}
	theGameState = MENU;
	theBtnType = EXIT;
	gameScore = 0;

	// Create textures for Game Dialogue (text)
	fontList = { "digital", "spaceAge" };
	fontsToUse = { "Fonts/digital-7.ttf", "Fonts/space age.ttf" };
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
	}
	gameTextList = { "Earth Defense", "Score: ", "Game Over", "Current Score Value", "Use arrow keys to rotate rocket"};

	theTextureMgr->addTexture("Title", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[0], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Score", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[1], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("GameOver", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[2], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("ScoreDisplay", theFontMgr->getFont("digital")->createTextTexture(theRenderer, gameTextList[3], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Controls", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[4], SOLID, { 0,255,0,255 }, { 0,0,0,0 }));

	// Load game sounds
	soundList = { "theme", "shot", "explosion", "click" };
	soundTypes = { MUSIC, SFX, SFX, SFX };
	soundsToUse = { "Audio/8-BitGame.mp3", "Audio/rocketMissileBlast.aiff", "Audio/blast.mp3", "Audio/beep.wav" };
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	theSoundMgr->getSnd("theme")->play(-1);
}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	 loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	switch (theGameState)
	{
	case MENU:
	{
		// Render the Title
		spriteBkgd.setSpritePos({ 0, 0 });
		spriteBkgd.setTexture(theTextureMgr->getTexture("menuBackground"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("menuBackground")->getTWidth(), theTextureMgr->getTexture("menuBackground")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		cTexture* tempTextTexture = theTextureMgr->getTexture("Title");
		SDL_Rect pos = { 335, 20, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		//display controls to the player
		cTexture* tempTextTexture4 = theTextureMgr->getTexture("Controls");
		SDL_Rect pos4{ 40, 500, tempTextTexture4->getTextureRect().w, tempTextTexture4->getTextureRect().h };
		FPoint scale4 = { 1,1 };
		tempTextTexture4->renderTexture(theRenderer, tempTextTexture4->getTexture(), &tempTextTexture4->getTextureRect(), &pos4, scale4);

		//render menu buttons
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 400, 375 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	}
	
	break;
	case PLAYING:
	{
		// Render the game scene
		spriteBkgd.setSpritePos({ 0, 0 });
		spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		// Render each meteor in the vector array
		for (int draw = 0; draw < theMeteors.size(); draw++)
		{
			theMeteors[draw]->render(theRenderer, &theMeteors[draw]->getSpriteDimensions(), &theMeteors[draw]->getSpritePos(), theMeteors[draw]->getSpriteRotAngle(), &theMeteors[draw]->getSpriteCentre(), theMeteors[draw]->getSpriteScale());
		}
		// Render each missile in the vector array
		for (int draw = 0; draw < theMissiles.size(); draw++)
		{
			theMissiles[draw]->render(theRenderer, &theMissiles[draw]->getSpriteDimensions(), &theMissiles[draw]->getSpritePos(), theMissiles[draw]->getSpriteRotAngle(), &theMissiles[draw]->getSpriteCentre(), theMissiles[draw]->getSpriteScale());
		}
		// render the rocket
		theRocket.setSpriteDimensions(theTextureMgr->getTexture("theRocket")->getTWidth(), theTextureMgr->getTexture("theRocket")->getTHeight());
		theRocket.render(theRenderer, &theRocket.getSpriteDimensions(), &theRocket.getSpritePos(), theRocket.getSpriteRotAngle(), &theRocket.getSpriteCentre(), theRocket.getSpriteScale());
		
		// Render the score text and update when score condition is met

		string pts = "Score : " + to_string(gameScore);
		LPCSTR temp = pts.c_str();
		theTextureMgr->deleteTexture("Score");
		theTextureMgr->addTexture("Score", theFontMgr->getFont("digital")->createTextTexture(theRenderer, temp, SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));

		cTexture* tempTextTexture2 = theTextureMgr->getTexture("Score");

		SDL_Rect pos2 = { 375, 35, tempTextTexture2->getTextureRect().w, tempTextTexture2->getTextureRect().h };
		FPoint scale2 = { 1, 1 };
		tempTextTexture2->renderTexture(theRenderer, tempTextTexture2->getTexture(), &tempTextTexture2->getTextureRect(), &pos2, scale2);
	}
	break;
	case END:
		{
		spriteBkgd.setSpritePos({ 0, 0 });
		spriteBkgd.setTexture(theTextureMgr->getTexture("endingBackground"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("endingBackground")->getTWidth(), theTextureMgr->getTexture("endingBackground")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 500, 350 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 500, 450 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());

		cTexture* tempTextTexture3 = theTextureMgr->getTexture("GameOver");
		SDL_Rect pos3 = { 375, 35, tempTextTexture3->getTextureRect().w, tempTextTexture3->getTextureRect().h };
		FPoint scale3 = { 1, 1 };
		tempTextTexture3->renderTexture(theRenderer, tempTextTexture3->getTexture(), &tempTextTexture3->getTextureRect(), &pos3, scale3);
		}
		break;
	case QUIT:
		{
		loop = false;
		}
		break;
	default:
		break;
	}
	
	SDL_RenderPresent(theRenderer);
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{

}

void cGame::update(double deltaTime)
{
	
		// Check Button clicked and change state
		switch (theGameState)
		{
			// Keep game running if the player doesn't quit
			if (theGameState != QUIT)
			{
				loop = true;
			}
		

		case MENU:
	{
			theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, PLAYING, theAreaClicked);
			theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
			theAreaClicked = { 0,0 };

			// make the rocket stationary when play is not pressed
			if (theGameState != PLAYING)
			{
				theRocket.setSpritePos({ 500, 350 });
				theRocket.setRocketVelocity({ 0,0 });
			}
			

			if (theGameState == PLAYING)
			{
				// Create vector array of textures
				for (int meteoroid = 0; meteoroid < 40; meteoroid++)
				{
					// spawn above render screen with random but wide ranged intervals
					theMeteors.push_back(new cMeteor);
					theMeteors[meteoroid]->setSpritePos({ 100 * (rand() % 8 + 1), -130 * (rand() % 85 + 1) });
					theMeteors[meteoroid]->setSpriteTranslation({ 0, 60});
					int randAsteroid = rand() % 4;
					theMeteors[meteoroid]->setTexture(theTextureMgr->getTexture(textureName[randAsteroid]));
					theMeteors[meteoroid]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randAsteroid])->getTWidth(), theTextureMgr->getTexture(textureName[randAsteroid])->getTHeight());
					theMeteors[meteoroid]->setMeteorVelocity({ 0, 0 });
					theMeteors[meteoroid]->setActive(true);
				}

				// play button sound when a change of game state occurs
				theSoundMgr->getSnd("click")->play(0);
				// reset score for each play iteration
				gameScore = 0;
			}
		}
			break;
		case PLAYING:
			{	
				if (theMeteors.size() <= 0)
				{
					theGameState = END;
				}
			}
			break;
		case END:
			{
				theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, MENU, theAreaClicked);
				theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
				theAreaClicked = { 0,0 };
				// stop rocket from moving after level completion
				if (theGameState != PLAYING)
				{
					theRocket.setSpritePos({ 500, 350 });
					theRocket.setRocketVelocity({ 0,0 });
				}
			}
			if (theGameState == MENU)
			{
				theSoundMgr->getSnd("click")->play(0);
			}
			if (theGameState == QUIT)
			{
				theSoundMgr->getSnd("click")->play(0);
			}
			break;
		case QUIT:
			{
			loop = false;
			}
		default:
			break;
		}
	
		// Update the visibility and position of each meteor
		vector<cMeteor*>::iterator asteroidIterator = theMeteors.begin();
		while (asteroidIterator != theMeteors.end())
	{
		if ((*asteroidIterator)->isActive() == false)
		{
			asteroidIterator = theMeteors.erase(asteroidIterator);
		}
		else
		{
			(*asteroidIterator)->update(deltaTime);
			++asteroidIterator;
		}
	}
	// Update the visibility and position of each missile
	vector<cMissile*>::iterator bulletIterartor = theMissiles.begin();
	while (bulletIterartor != theMissiles.end())
	{
		if ((*bulletIterartor)->isActive() == false)
		{
			bulletIterartor = theMissiles.erase(bulletIterartor);
		}
		else
		{
			(*bulletIterartor)->update(deltaTime);
			++bulletIterartor;
		}
	}
	/*
	==============================================================
	| Check for collisions
	==============================================================
	*/
	for (vector<cMissile*>::iterator bulletIterartor = theMissiles.begin(); bulletIterartor != theMissiles.end(); ++bulletIterartor)
	{
		for (vector<cMeteor*>::iterator asteroidIterator = theMeteors.begin(); asteroidIterator != theMeteors.end(); ++asteroidIterator)
		{
			if ((*asteroidIterator)->collidedWith(&(*asteroidIterator)->getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
			{
				// if a collision set the bullet and asteroid to false
				(*asteroidIterator)->setActive(false);
				(*bulletIterartor)->setActive(false);
				theSoundMgr->getSnd("explosion")->play(0);
				gameScore = gameScore + 100;
			}
			
		}
	}


	// Update the Rockets position
	theRocket.update(deltaTime);
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
				theAreaClicked = { event.motion.x, event.motion.y };
			}
			break;
			case SDL_BUTTON_RIGHT:
				break;
			default:
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
			}
			break;
			case SDL_BUTTON_RIGHT:
				break;
			default:
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				theLoop = false;
				break;
			case SDLK_DOWN:
			{
				
			}
			break;

			case SDLK_UP:
			{
				
			}
			break;
			case SDLK_RIGHT:
			{
				theRocket.setSpriteRotAngle(theRocket.getSpriteRotAngle() + 10);
			}
			break;

			case SDLK_LEFT:
			{
				theRocket.setSpriteRotAngle(theRocket.getSpriteRotAngle() - 10);
			}
			break;
			case SDLK_SPACE:
			{
				theMissiles.push_back(new cMissile);
				int numBullets = theMissiles.size() - 1;
				theMissiles[numBullets]->setSpritePos({ theRocket.getBoundingRect().x + theRocket.getSpriteCentre().x, theRocket.getBoundingRect().y + theRocket.getSpriteCentre().y });
				theMissiles[numBullets]->setSpriteTranslation({ 2, 6 });
				theMissiles[numBullets]->setTexture(theTextureMgr->getTexture("bullet"));
				theMissiles[numBullets]->setSpriteDimensions(theTextureMgr->getTexture("bullet")->getTWidth(), theTextureMgr->getTexture("bullet")->getTHeight());
				theMissiles[numBullets]->setMissileVelocity({ 4, 4 });
				theMissiles[numBullets]->setSpriteRotAngle(theRocket.getSpriteRotAngle());
				theMissiles[numBullets]->setActive(true);
			}
			theSoundMgr->getSnd("shot")->play(0);
			break;
			default:
				break;
			}

		default:
			break;
		}

	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

