#pragma once
#include <iostream>
#include <fstream>
#include "SceneMain.h"
#include "GameObjects.h"
#include "Sections.h"

class GameScene : public SceneMain
{
protected:
	LPGAMEOBJECT player;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_STATE_ANIMATION(string line);
	void _ParseSection_OBJECT_ANIMATIONS(string line);
	void _ParseSection_COLLISION_BOXES(string line);
	void _ParseSection_SECTIONS(string line);
	void _ParseSection_CLASSES(string line);
	void _ParseSection_OBJECTS(string line);

public:
	GameScene(int id, LPCWSTR filePath, int startupSection = -1);

	void InitSaveGameSideView();
	void InitSaveGameOverhead();

	unordered_map<int, LPSECTION> Sections;
	int CurrentSectionId;
	int NextSectionId;
	LPPORTAL fromPortal;
	LPPORTAL toPortal;
	float offset_x_SectionSwitch;
	float offset_y_SectionSwitch;

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	void HandleKeys(DWORD dt);
	void HandleKeyDown(DWORD dt, int keyCode);

	void SetPlayer(LPGAMEOBJECT newPlayer) { player = newPlayer; }
	LPGAMEOBJECT GetPlayer() { return player; } 

	float camBoxLeft, camBoxRight, camBoxTop, camBoxBottom;
	void init_camBox();
	void update_camBox();

	LPSECTION GetCurrentSection() { return Sections[CurrentSectionId]; }
	bool isSectionSwitch();

	void CreatePosCameraFollowPlayer(float& cx, float& cy);
	void MoveCameraBeforeSwitchSection(float& cx, float& cy);
	void PreventCameraOverBoundingBox(float& cx, float& cy);
	void ResetGameStateAfterSwichtSection();

	~GameScene();
};

typedef GameScene* LPGAMESCENE;

