﻿#include "Game.h"
#include "GameScene.h"
#include "Utils.h"
#include <math.h> 
#include <time.h>
#include <vector>

//#include "SoundManager.h"

//#define SCENE_SIDEVIEW_ID 2

CGame* CGame::__instance = nullptr;

int CGame::state = 1;
D3DCOLOR CGame::BackgroundColor = D3DCOLOR_XRGB(255, 255, 255);


/*
	Initialize DirectX, create a Direct3D device for rendering within the window, initial Sprite library for
	rendering 2D images
	- hInst: Application instance handle
	- hWnd: Application window handle
*/
void CGame::Init(HWND hWnd)
{
	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);

	this->hWnd = hWnd;

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;

	RECT r;
	GetClientRect(hWnd, &r);	// retrieve Window width & height 

	d3dpp.BackBufferHeight = r.bottom + 1;
	d3dpp.BackBufferWidth = r.right + 1;

	screen_height = r.bottom + 1;
	screen_width = r.right + 1;

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddv);

	if (d3ddv == nullptr)
	{
		OutputDebugString(L"[ERROR] CreateDevice failed\n");
		return;
	}

	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	// Initialize sprite helper from Direct3DX helper library
	D3DXCreateSprite(d3ddv, &spriteHandler);

	srand(time(NULL));

	OutputDebugString(L"[INFO] InitGame done;\n");
}

HWND CGame::getCurrentHWND()
{
	return hWnd;
}

void CGame::Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha, bool flipX, int rotate, float offset_x, float offset_y, int modifyR, int modifyG, int modifyB)
{
	int width = round(right - left);
	int height = round(bottom - top);
	int scale = 1;
	D3DXVECTOR2 center = D3DXVECTOR2((int)(flipX ? (width / 2) * scale - width * scale : (width / 2) * scale), (int)((height / 2) * scale));
	D3DXVECTOR2 translate = D3DXVECTOR2((int)(flipX ? x + width * scale - cam_x - offset_x : x - cam_x - offset_x), (int)(y - cam_y - offset_y));
	D3DXVECTOR2 scaling = D3DXVECTOR2((int)((flipX) ? -scale : scale), scale);
	float angle = rotate * 1.5707963268;
	RECT r;
	r.left = left;
	r.top = top;
	r.right = right;
	r.bottom = bottom;
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix,
		NULL,
		0.0f,
		&scaling,
		&center,
		angle,
		&translate
	);
	spriteHandler->SetTransform(&matrix);
	spriteHandler->Draw(texture, &r, NULL, NULL, D3DCOLOR_ARGB(alpha, modifyR, modifyG, modifyB));
}

int CGame::IsKeyDown(int KeyCode)
{
	return (keyStates[KeyCode] & 0x80) > 0;
}

void CGame::InitKeyboard()
{
	HRESULT
		hr = DirectInput8Create
		(
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8, (VOID**)&di, nullptr
		);

	if (hr != DI_OK)
	{
		return;
	}

	hr = di->CreateDevice(GUID_SysKeyboard, &didv, nullptr);

	if (hr != DI_OK)
	{
		return;
	}

	hr = didv->SetDataFormat(&c_dfDIKeyboard);

	hr = didv->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE;

	hr = didv->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	hr = didv->Acquire();
	if (hr != DI_OK)
	{
		return;
	}
}

void CGame::ProcessKeyboard()
{
	HRESULT hr;

	// Collect all key states first
	hr = didv->GetDeviceState(sizeof(keyStates), keyStates);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			HRESULT h = didv->Acquire();
			if (h == DI_OK)
			{
			}
			else return;
		}
		else
		{
			//DebugOut(L"[ERROR] DINPUT::GetDeviceState failed. Error: %d\n", hr);
			return;
		}
	}

	// Collect all buffered events
	DWORD dwElements = KEYBOARD_BUFFER_SIZE;
	hr = didv->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), keyEvents, &dwElements, 0);
	if (FAILED(hr))
	{
		//DebugOut(L"[ERROR] DINPUT::GetDeviceData failed. Error: %d\n", hr);
		return;
	}


	// Scan through all buffered events, check if the key is pressed or released
	for (DWORD i = 0; i < dwElements; i++)
	{
		int KeyCode = keyEvents[i].dwOfs;
		int KeyState = keyEvents[i].dwData;
	}
}

CGame::~CGame()
{
	if (spriteHandler != nullptr) spriteHandler->Release();
	if (backBuffer != nullptr) backBuffer->Release();
	if (d3ddv != nullptr) d3ddv->Release();
	if (d3d != nullptr) d3d->Release();
}

GameObject* CGame::GetCurrentPlayer()
{
	LPGAMESCENE currentGameScene = dynamic_cast<LPGAMESCENE>(scenes[current_scene]);

	if (currentGameScene)
		return currentGameScene->GetPlayer();

	return nullptr;
}

void CGame::SetState(int newState)
{
	state = newState;
}

CGame* CGame::GetInstance()
{
	if (__instance == nullptr) __instance = new CGame();
	return __instance;
}

#define MAX_GAME_LINE 1024


#define GAME_FILE_SECTION_UNKNOWN -1
#define GAME_FILE_SECTION_SETTINGS 1
#define GAME_FILE_SECTION_SCENES 2

void CGame::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	if (tokens[0] == "start")
		current_scene = atoi(tokens[1].c_str());
}

void CGame::_ParseSection_SCENES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	int id = atoi(tokens[0].c_str());
	LPCWSTR path = ToLPCWSTR(tokens[1]);

	LPSCENE scene = NULL;

	scenes[id] = scene;
}

/*
	Load game campaign file and load/initiate first scene
*/
void CGame::Load(LPCWSTR gameFile)
{
	ifstream f;
	f.open(gameFile);
	char str[MAX_GAME_LINE];

	// current resource section flag
	int section = GAME_FILE_SECTION_UNKNOWN;

	while (f.getline(str, MAX_GAME_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SETTINGS]") { section = GAME_FILE_SECTION_SETTINGS; continue; }
		if (line == "[SCENES]") { section = GAME_FILE_SECTION_SCENES; continue; }

		//
		// data section
		//
		switch (section)
		{
		case GAME_FILE_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
		case GAME_FILE_SECTION_SCENES: _ParseSection_SCENES(line); break;
		}
	}
	f.close();

	SwitchScene(current_scene);
}

void CGame::SwitchScene(int scene_id)
{
	scenes[current_scene]->Unload();

	current_scene = scene_id;
	LPSCENE s = scenes[scene_id];
	s->Load();
}
