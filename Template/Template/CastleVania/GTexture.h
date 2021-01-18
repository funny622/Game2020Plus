#pragma once
#include <unordered_map>
#include <d3dx9.h>
#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>
#include "Game.h"
#include "Utils.h"
#include "GTexture.h"

using namespace std;

/*
	Manage texture database
*/
class GTexture
{
	static GTexture* __instance;

	unordered_map<int, LPDIRECT3DTEXTURE9> textures;

public:
	GTexture();
	void Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor);
	LPDIRECT3DTEXTURE9 Get(unsigned int i);

	void Clear();
	static GTexture* GetInstance();
};