#pragma once
#include <d3d9.h>
#include "Game.h"
#include "GTexture.h"
#include "GameObjects.h"
#include "Portal.h"
#include "GridObjects.h"

class CSection
{
private:
	// int sectionId;					we may not need this
	int backgroundTextureId;
	int foregroundTextureId;
	// LPSCENE scene;					why we need to know its scene anyway?
	int bgWidth, bgHeight;

public:
	CSection() {};
	CSection(int bgTextureId, int fgTextureId);

	int getBgWidth() { return bgWidth; };
	int getBgHeight() { return bgHeight; };
	void Update(DWORD dt);

	CGridObjects* gridObjects = nullptr;

	/*void CSection::addObject(LPGAMEOBJECT obj);*/
	void addObject(LPGAMEOBJECT obj);

	LPPORTAL findScenePortal(int port);
};

typedef CSection* LPSECTION;
