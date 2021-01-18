#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>
#include "Utils.h"
#include "Game.h"
#include <iostream>

using namespace std;

class GSprite
{
	int id;		

	int left;
	int top;
	int right;
	int bottom;

	LPDIRECT3DTEXTURE9 texture;
public:
	GSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);

	void Draw(float x, float y, int alpha = 255, bool flipX = false, int rotate = 0, int modifyR = 255, int modifyG = 255, int modifyB = 255);
};

typedef GSprite* LPSPRITE;

class GSpriteLib
{
	static GSpriteLib* __instance;

	unordered_map<int, LPSPRITE> sprites;

public:
	void Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);
	LPSPRITE Get(int id);
	void GSpriteLib::Clear();

	static GSpriteLib* GetInstance();
};



