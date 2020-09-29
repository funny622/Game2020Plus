
#include "SceneMain.h"

SceneMain::SceneMain(int _nCmdShow): CGame(_nCmdShow)
{
}

void SceneMain::RenderFrame(LPDIRECT3DDEVICE9 d3ddv, int t)
{
	//d3ddv->StretchRect(
	//	Background,			// from 
	//	NULL,				// which portion?
	//	G_BackBuffer,		// to 
	//	NULL,				// which portion?
	//	D3DTEXF_NONE);

	d3ddv->ColorFill(G_BackBuffer, NULL, D3DCOLOR_XRGB(0, 0, 0));

	G_SpriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
	
	//Do??

	G_SpriteHandler->End();
}

void SceneMain::ProcessInput(LPDIRECT3DDEVICE9 d3ddv, int Delta)
{
}

void SceneMain::LoadResources(LPDIRECT3DDEVICE9 d3ddv)
{
}

void SceneMain::OnKeyDown(int KeyCode)
{

}

SceneMain::~SceneMain(void)
{
}
