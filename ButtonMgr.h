#pragma once

#ifndef _BUTTONMGR_H
#define _BUTTONMGR_H

//OpenGL Headers
#include "GameConstants.h"
#include "Button.h"


using namespace std;

class cButtonMgr
{
private:
	static cButtonMgr* pInstance;
protected:
	cButtonMgr();
	~cButtonMgr();
	map <LPCSTR, cButton*> gameBtns;
public:
	void add(LPCSTR BtnName, cButton* theBtn);
	cButton* getBtn(LPCSTR btnName);
	void deleteBtn();
	static cButtonMgr* getInstance();
};
#endif