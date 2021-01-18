#include "GSprite.h"

GSprite::GSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
}

GSpriteLib* GSpriteLib::__instance = nullptr;

GSpriteLib* GSpriteLib::GetInstance()
{
	if (__instance == nullptr) __instance = new GSpriteLib();
	return __instance;
}

void GSprite::Draw(float x, float y, int alpha, bool flipX, int rotate, int modifyR, int modifyG, int modifyB)
{
	CGame* game = CGame::GetInstance();
	game->Draw(x, y, texture, left, top, right, bottom, alpha, flipX, rotate, 0, 0, modifyR, modifyG, modifyB);
}

void GSpriteLib::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	LPSPRITE s = new GSprite(id, left, top, right, bottom, tex);
	sprites[id] = s;

	DebugOut(L"[INFO] sprite added: %d, %d, %d, %d, %d \n", id, left, top, right, bottom);
}

LPSPRITE GSpriteLib::Get(int id)
{
	return sprites[id];
}

/*
	Clear all loaded textures
*/
void GSpriteLib::Clear()
{
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}



