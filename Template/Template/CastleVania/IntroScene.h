#pragma once
#include "SceneMain.h";
#include <iostream>
#include <fstream>
#include "Animations.h"

class CIntroScene : public SceneMain
{
protected:
	//Sanh code Input Sence
	int state;
	bool isTitleFinished = false; // true is finished
	bool isFilmFinished = false;
	bool isIntroFinished = false;
	bool isLeftFinished = false;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_STATE_ANIMATION(string line);
	void _ParseSection_OBJECT_ANIMATIONS(string line);

public:
	void setState(int state) { this->state = state; }
	CObjectAnimationHanlders animationHandlers;
	void HandleKeyEnter();
	CIntroScene(int id, LPCWSTR filePath, int startupSection = -1);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	//Reset Game
	void pressContinue();
	void pressEnd();
};

