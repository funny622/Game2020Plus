#include "Sections.h"
#include "TileArea.h"
#include "Sophia.h"
#include "GameManager.h"
#include <stdlib.h>    
#include <time.h>  


CSection::CSection(int bgTextureId, int fgTextureId)
{
	this->backgroundTextureId = bgTextureId;
	this->foregroundTextureId = fgTextureId;

	LPDIRECT3DTEXTURE9 backgroundTexture = GTexture::GetInstance()->Get(backgroundTextureId);

	// getting the size of texture
	D3DSURFACE_DESC surfaceDesc;
	backgroundTexture->GetLevelDesc(0, &surfaceDesc);

	bgWidth = surfaceDesc.Width;
	bgHeight = surfaceDesc.Height;

	gridObjects = new CGridObjects(bgWidth, bgHeight);
}

void CSection::Update(DWORD dt)
{
	// CuteTN to do: filter out the neccessary objects to update

	// collision objects for static object, i.e. tiles
	//vector<LPGAMEOBJECT> coObjs_static;
	//coObjs_static.clear();

	float cx, cy, cw, ch;
	CGame::GetInstance()->GetCamPos(cx, cy);
	cw = CGame::GetInstance()->GetScreenWidth();
	ch = CGame::GetInstance()->GetScreenHeight();
}

LPPORTAL CSection::findScenePortal(int port)
{
	return nullptr;
}

void CSection::addObject(LPGAMEOBJECT obj)
{
	if (!obj)
		return;

	gridObjects->Add(obj);
}

void CSection::Render(float offset_x, float offset_y)
{
	float cx, cy, cw, ch;
	CGame::GetInstance()->GetCamPos(cx, cy);
	cw = CGame::GetInstance()->GetScreenWidth();
	ch = CGame::GetInstance()->GetScreenHeight();

	vector<LPGAMEOBJECT> objects = gridObjects->GetObjectsInArea(cx, cy, cw, ch);

	// CuteTN Note: the order of rendering would be implemented here :)
	RenderTexture(backgroundTextureId, offset_x, offset_y);

	for (auto obj : objects)
	{
		if (checkObjInCamera(obj, SCREEN_EXTEND_OFFSET_DEFAULT))
			if (obj->isHiddenByForeground)
				obj->Render(offset_x, offset_y);
	}

	RenderTexture(foregroundTextureId, offset_x, offset_y);

	for (auto obj : objects)
	{
		if (checkObjInCamera(obj, SCREEN_EXTEND_OFFSET_DEFAULT))
			if (!obj->isHiddenByForeground)
				obj->Render(offset_x, offset_y);
	}
}

void CSection::RenderTexture(int textureId, float offset_x, float offset_y)
{
	LPDIRECT3DTEXTURE9 backgroundTexture = GTexture::GetInstance()->Get(textureId);

	//EFFECT BOSS - SANHLIKE CUTE
	CGameManager* manager = CGameManager::GetInstance();

	if (manager->isEffectBoss)
	{
		int R = rand() % 255;
		int G = rand() % 255;
		int B = rand() % 255;
		int A = 255;
		CGame::GetInstance()->Draw(offset_x, offset_y, backgroundTexture, 0, 0, bgWidth, bgHeight, A, false, 0, 0, 0, R, G, B);
	}
	else
		CGame::GetInstance()->Draw(offset_x, offset_y, backgroundTexture, 0, 0, bgWidth, bgHeight);
}

