#pragma once
#include <iostream>
#include <fstream>
#include "SceneMain.h"
#include "GameObjects.h"

class GameScene : public SceneMain
{
protected:
	LPGAMEOBJECT player;

	//void _ParseSection_TEXTURES(string line);
	//void _ParseSection_SPRITES(string line);
	//void _ParseSection_ANIMATIONS(string line);
	//void _ParseSection_STATE_ANIMATION(string line);
	//void _ParseSection_OBJECT_ANIMATIONS(string line);
	//void _ParseSection_COLLISION_BOXES(string line);
	//void _ParseSection_SECTIONS(string line);
	//void _ParseSection_CLASSES(string line);
	//void _ParseSection_OBJECTS(string line);

	int CurrentSectionId;
public:
	GameScene(int id, LPCWSTR filePath, int startupSection = -1);

	//virtual void Load();
	//virtual void Update(DWORD dt);
	//virtual void Render();
	//virtual void Unload();

	void HandleKeys(DWORD dt);
	void HandleKeyDown(DWORD dt, int keyCode);

	void SetPlayer(LPGAMEOBJECT newPlayer) { player = newPlayer; }
	LPGAMEOBJECT GetPlayer() { return player; } 

	~GameScene();
};

typedef GameScene* LPGAMESCENE;

