#include "GameScene.h"
#include "GTexture.h"
#include "GSprite.h"
#include "Animations.h"
#include "Portal.h"
#include "PortalLib.h"
#include "Sections.h"
using namespace std;

GameScene::GameScene(int id, LPCWSTR filePath, int startupSectionId) : SceneMain(id, filePath)
{
	CurrentSectionId = startupSectionId;
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_STATE_ANIMATION	5
#define SCENE_OBJECT_ANIMATION	6
#define SCENE_COLLISION_BOXES	7
#define SCENE_SECTIONS	8
#define SCENE_CLASSES	9
#define SCENE_OBJECTS	10
#define MAX_SCENE_LINE 1024

void GameScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	//int R = atoi(tokens[2].c_str());
	//int G = atoi(tokens[3].c_str());
	//int B = atoi(tokens[4].c_str());

	int R = 254;
	int G = 0;
	int B = 0;

	GTexture::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void GameScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int texID = atoi(tokens[1].c_str());
	int l = atoi(tokens[2].c_str());
	int t = atoi(tokens[3].c_str());
	int r = atoi(tokens[4].c_str());
	int b = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = GTexture::GetInstance()->Get(texID);
	if (tex == nullptr)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	GSpriteLib::GetInstance()->Add(ID, l, t, r, b, tex);
}

void GameScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	////DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int duration = atoi(tokens[i + 1].c_str());

		DebugOut(L"spriteId: %d   duration: %d\n", sprite_id, duration);
		ani->AddFrame(sprite_id, duration);
	}

	CAnimationLib::GetInstance()->Add(ani_id, ani);
}

void GameScene::_ParseSection_STATE_ANIMATION(string line)
{
	//Sanh
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return;

	int state_id = atoi(tokens[0].c_str());
	int ani_id = atoi(tokens[1].c_str());
	LPANIMATION ani = CAnimationLib::GetInstance()->Get(ani_id);

	int flipX = atoi(tokens[2].c_str());
	int flipY = atoi(tokens[3].c_str());
	int rotate = atoi(tokens[4].c_str());

	// Add to lib
	CAnimationHandlersLib::GetInstance()->Add(state_id, ani, flipX, flipY, rotate);
	DebugOut(L"[INFO] Added StateId %d, animId %d, flipX %d, flipY %d, rotate %d\n", state_id, ani_id, flipX, flipY, rotate);
}

void GameScene::_ParseSection_OBJECT_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;

	int objectAni_id = atoi(tokens[0].c_str());
	LPOBJECT_ANIMATIONS objAni = new CObjectAnimations();

	for (int i = 1; i < tokens.size(); i++)
	{
		int stateId = atoi(tokens[i].c_str());
		objAni->AddState(stateId);
	}

	// Add to lib
	CObjectAnimationsLib::GetInstance()->Add(objectAni_id, objAni);
}

void GameScene::_ParseSection_COLLISION_BOXES(string line)
{
	// CuteTN Note: Empty functions since we no more need to import collision box :)
}

void GameScene::_ParseSection_SECTIONS(string line)
{
	//Sanh
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines

	int section_ID = atoi(tokens[0].c_str());
	int background_ID = atoi(tokens[1].c_str());
	int foreground_ID = atoi(tokens[2].c_str());

	LPDIRECT3DTEXTURE9 tempTxture = GTexture::GetInstance()->Get(background_ID);
	if (tempTxture == nullptr)
	{
		DebugOut(L"[ERROR] Background of section %d not found!\n", section_ID);
		return;
	}

	tempTxture = GTexture::GetInstance()->Get(foreground_ID);
	if (tempTxture == nullptr)
	{
		DebugOut(L"[ERROR] Foreground of section %d not found!\n", section_ID);
		return;
	}


	//Add section
	this->Sections[section_ID] = new CSection(background_ID, foreground_ID);
}

void GameScene::_ParseSection_CLASSES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines

	int class_ID = atoi(tokens[0].c_str());
	int objAnims_ID = atoi(tokens[1].c_str());

	if (objAnims_ID != -1)
	{
		CObjectAnimationsLib::GetInstance()->AddClass(class_ID, objAnims_ID);
	}
}

void GameScene::_ParseSection_OBJECTS(string line)
{
	//Sanh
	vector<string> tokens = split(line);

	if (tokens.size() < 4) return; // skip invalid lines

	int obj_ID = atoi(tokens[0].c_str());
	int class_ID = atoi(tokens[1].c_str());

	//Create list properties 
	map<string, string> Properties;
	for (int i = 2; i < tokens.size(); i += 2)
	{
		string prop_name = tokens[i].c_str();
		string prop_value = tokens[i + 1].c_str();

		Properties[prop_name] = prop_value;
	}

	// Create a new game object
	int sectionId;
	LPGAMEOBJECT obj = CGameObjectFactory::Create(class_ID, Properties, sectionId);

	// current player
	if (class_ID == CLASS_SOPHIA && CGame::GetInstance()->GetCurrentSceneId() == ID_SCENE_SIDEVIEW)
	{
		// CuteTN To do: may us delete this later?
		// set the first section as startup
		if (CurrentSectionId == -1)
			CurrentSectionId = obj->currentSectionId;

		this->player = obj;
	}

	if (obj == nullptr)
		DebugOut(L"[ERROR] Cannot create object with object Id: %d\n", obj_ID);
	else
	{
		obj->objectId = obj_ID;
		// Sections[sectionId]->Objects.push_back(obj);
		Sections[sectionId]->addObject(obj);

		// add portal to library
		if (obj->classId == CLASS_TILE_PORTAL)
		{
			CPortalLib::GetInstance()->Add(obj_ID, dynamic_cast<LPPORTAL>(obj));
			DebugOut(L"[INFO] Add Portal to Lib: %d of section %d\n", obj_ID, obj->currentSectionId);
		}
	}
}
