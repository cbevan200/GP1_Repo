/*
	=========
	cButtonMgr.cpp
	- CPP file for class definition - IMPLEMENTATION
	- CPP file for the ButtonMgr class
	=========
*/
#include "ButtonMgr.h"
cButtonMgr* cButtonMgr::pInstance = NULL;

/*
==============
Singleton design pattern
==============
*/
cButtonMgr* cButtonMgr::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cButtonMgr();
	}
	return cButtonMgr::pInstance;
}
/*
	==========
	- Data constructor initialises the ButtonMgr object
	- Is always called, therefore all ButtonMgr objects are in a consistent state
	==========
*/
cButtonMgr::cButtonMgr()
{

}
/*
=========
- Destructor
=========
*/
cButtonMgr::~cButtonMgr()
{
	deleteBtn();
}

void cButtonMgr::add(LPCSTR btnName, cButton* theBtn)
{
	if (!getBtn(btnName))
	{
		// create new button instance
		// add assinged texture to that button
		gameBtns.insert(make_pair(btnName, theBtn));
	}
}

void cButtonMgr::deleteBtn()
{
	for (map<LPCSTR, cButton*>::iterator btn = gameBtns.begin(); btn != gameBtns.end(); ++btn)
	{
		delete btn->second;
	}
}

/*
==========
- return the texture
==========
*/

cButton* cButtonMgr::getBtn(LPCSTR btnName) 
{
	map<LPCSTR, cButton*>::iterator btn = gameBtns.find(btnName);
	if (btn != gameBtns.end())
	{
		return btn->second;
	}
	else
	{
		return NULL;
	}
}