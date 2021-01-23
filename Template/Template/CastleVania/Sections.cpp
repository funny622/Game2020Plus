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
