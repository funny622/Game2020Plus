#include "GameScene.h"
#include "GTexture.h"
#include "GSprite.h"
#include "Animations.h"
#include "Portal.h"
#include "PortalLib.h"
#include "Sections.h"

#include "GameObjectFactory.h"
#include "GameState.h"
#include "Game.h"
#include "GameManager.h"

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

void GameScene::Load()
{
	CGameManager::GetInstance()->initEffectFaded();
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[STATE_ANIMATIONS]") { section = SCENE_SECTION_STATE_ANIMATION; continue; }
		if (line == "[OBJECT_ANIMATIONS]") { section = SCENE_OBJECT_ANIMATION; continue; }
		if (line == "[COLLISION_BOXES]") { section = SCENE_COLLISION_BOXES; continue; }
		if (line == "[SECTIONS]") { section = SCENE_SECTIONS; continue; }
		if (line == "[CLASSES]") { section = SCENE_CLASSES; continue; }
		if (line == "[OBJECTS]") { section = SCENE_OBJECTS; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }
		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line);   break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_STATE_ANIMATION: _ParseSection_STATE_ANIMATION(line);  break;
		case SCENE_OBJECT_ANIMATION: _ParseSection_OBJECT_ANIMATIONS(line);  break;
		case SCENE_COLLISION_BOXES: _ParseSection_COLLISION_BOXES(line);  break;
		case SCENE_SECTIONS: _ParseSection_SECTIONS(line);  break;
		case SCENE_CLASSES: _ParseSection_CLASSES(line); break;
		case SCENE_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	//CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

	int currentSceneId = CGame::GetInstance()->GetCurrentSceneId();

	if (currentSceneId == ID_SCENE_SIDEVIEW)
	{
		CGame::GetInstance()->SetState(GameState::PLAY_SIDEVIEW_SOPHIA);
		InitSaveGameSideView();
	}
	else if (currentSceneId == ID_SCENE_OVERHEAD)
	{
		CGame::GetInstance()->SetState(GameState::PLAY_OVERHEAD);
		InitSaveGameOverhead();
	}

	init_camBox();
	//play(name, isLoop, times)
}

void GameScene::init_camBox()
{
	float playerX, playerY;
	CGame::GetInstance()->GetCurrentPlayer()->GetPosition(playerX, playerY);
	camBoxLeft = playerX;
	camBoxRight = playerX + 16 * 4;
	camBoxTop = playerY - 16 * 4;
	camBoxBottom = playerY + 32;

	//Fix camera lag
	//float cx, cy;
	//CGame* game = CGame::GetInstance();
	//game->GetCamPos(cx, cy);
	//camBoxLeft = game->GetScreenWidth() / 2 - 8 + cx - 16 * 2;
	//camBoxBottom = game->GetScreenHeight() - 16 + cy - 16 * 3;
}

void GameScene::update_camBox()
{
	float playerX, playerY;
	CGame::GetInstance()->GetCurrentPlayer()->GetPosition(playerX, playerY);

	bool isJason = false;
	bool isSophia = false;
	bool isJasonOverhead = false;
	if (CGame::GetInstance()->GetCurrentPlayer()->classId == CLASS_SOPHIA) isSophia = true;
	if (CGame::GetInstance()->GetCurrentPlayer()->classId == CLASS_JASONSIDEVIEW) isJason = true;
	if (CGame::GetInstance()->GetCurrentPlayer()->classId == CLASS_JASONOVERHEAD) isJasonOverhead = true;

	if (playerX + 16 * 2 >= camBoxRight) {
		camBoxRight = playerX + 16 * 2;
		camBoxLeft = playerX - 16 * 2;
	}

	if ((isSophia || isJasonOverhead) && playerX <= camBoxLeft)
	{
		camBoxLeft = playerX;
		camBoxRight = playerX + 16 * 4;
	}

	if (isJason && playerX <= camBoxLeft + 8)
	{
		camBoxLeft = playerX - 8;
		camBoxRight = playerX + 16 * 4;
	}

	if (playerY + 32 >= camBoxBottom) {
		camBoxBottom = playerY + 32;
		camBoxTop = camBoxBottom - 16 * 6;
	}

	if (playerY - 16 <= camBoxTop) {
		camBoxTop = playerY - 16;
		camBoxBottom = camBoxTop + 16 * 6;
	}
}

void GameScene::InitSaveGameSideView()
{
	//Init follow save Game
	CGameManager* manager = CGameManager::GetInstance();

	manager->resetHealth();

	if (manager->isSaved() == false)
		return;

	if (manager->getPlayer() == 2)
		CGame::GetInstance()->SetState(GameState::PLAY_SIDEVIEW_JASON);

	init_camBox();
}

void GameScene::InitSaveGameOverhead()
{
	DebugOut(L"\n init over");
	CGame::GetInstance()->SetState(GameState::PLAY_OVERHEAD);
	CGameManager* manager = CGameManager::GetInstance();

	if (manager->getPlayer() != 3) // Khong phai reset o overhead 
		return;

	float xPlayer, yPlayer;
	int idSection = 0;

	CurrentSectionId = idSection;

	DebugOut(L"\n get info");
}

bool GameScene::isSectionSwitch()
{
	if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_LEFT)
		return true;
	if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_RIGHT)
		return true;
	if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_LEFT_JASON)
		return true;
	if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_RIGHT_JASON)
		return true;
	if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_OVERHEAD_LEFT)
		return true;
	if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_OVERHEAD_RIGHT)
		return true;
	if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_OVERHEAD_UP)
		return true;
	if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_OVERHEAD_DOWN)
		return true;
	return false;
}

void GameScene::Update(DWORD dt)
{
	//WARNINGGGGGGGGGGGGGGGGGGGGGG "RETURN"
	//Can't update objs while menu wepon open 
	HandleKeys(dt);
	CGameManager* manager = CGameManager::GetInstance();
	manager->UpdateEffect(dt);
	if (manager->isMenuWeaponOpen())
	{
		manager->Update(dt);
		return;
	}

	bool isNarrowSection = false;
	if (Sections[CurrentSectionId]->getBgHeight() <= 400)
		isNarrowSection = true;

	bool isSectionSwitch = GameScene::isSectionSwitch();

	float cx, cy;
	CreatePosCameraFollowPlayer(cx, cy);

	if (isSectionSwitch)
	{
		CGame::GetInstance()->GetCamPos(cx, cy);
		MoveCameraBeforeSwitchSection(cx, cy);
	}
	PreventCameraOverBoundingBox(cx, cy);
	//Fixed position cy = 16 with narrow section
	if (isNarrowSection && (CGame::GetInstance()->GetCurrentPlayer()->classId != CLASS_JASONOVERHEAD) && !isSectionSwitch)
		cy = 16;
	if (isNarrowSection && (CGame::GetInstance()->GetCurrentPlayer()->classId == CLASS_JASONOVERHEAD) && !isSectionSwitch)
		cy = 0;

	//Fix Overhead bounding

	LPSECTION section = this->GetCurrentSection();
	float width_section = section->getBgWidth();
	float height_section = section->getBgHeight();
	CGame* game = CGame::GetInstance();
	GameState _gameState = game->GetState();

	if (_gameState == GameState::PLAY_OVERHEAD && cy + game->GetScreenHeight() >= height_section - 32)
	{
		cy = height_section - 32 - game->GetScreenHeight();

	}



	CGame::GetInstance()->SetCamPos(cx, cy);

	if (isSectionSwitch)
		ResetGameStateAfterSwichtSection();

	Sections[CurrentSectionId]->Update(dt);
	// CuteTN to do: switching section here
}

void GameScene::Render()
{
	Sections[CurrentSectionId]->Render();
	//SANH-CAMERA
	if (GameScene::isSectionSwitch())
	{
		Sections[NextSectionId]->Render(this->offset_x_SectionSwitch, this->offset_y_SectionSwitch);
	}
	CGameManager* manager = CGameManager::GetInstance();

	//HEA * global = CGameGlobal::GetInstance();LTH POW

	if (player->classId == CLASS_JASONOVERHEAD)
		manager->RenderHealthGun();
	else
		manager->RenderHealth();
	manager->RenderWeapon();

	//Render effect
	manager->RenderEffect();
}

void GameScene::Unload()
{
	CurrentSectionId = -1;
}

void GameScene::HandleKeys(DWORD dt)
{
	auto keyEvents = NewKeyEvents();

	for (auto e : keyEvents)
	{
		int keyCode = e->GetKeyCode();
		if (e->IsDown())
			HandleKeyDown(dt, keyCode);
	}
}

void GameScene::HandleKeyDown(DWORD dt, int keyCode)
{
	//Sanh - OPEN WEPON MENU 
	CGameManager* manager = CGameManager::GetInstance();
	if (keyCode == DIK_RETURN && !manager->isMenuWeaponOpen())
	{
		manager->OpenMenuWeapon();
		return;
	}
	if (keyCode == DIK_RETURN && manager->isMenuWeaponOpen())
	{
		manager->CloseMenuWeapon();
	}
	if ((keyCode == DIK_RIGHT || keyCode == DIK_UP) && manager->isMenuWeaponOpen())
	{
		manager->NextSelectedItem();
	}
	if ((keyCode == DIK_LEFT || keyCode == DIK_DOWN) && manager->isMenuWeaponOpen())
	{
		manager->BackSelectedItem();
	}
}

void GameScene::CreatePosCameraFollowPlayer(float& cx, float& cy)
{
	this->update_camBox();
	LPSECTION section = this->GetCurrentSection();
	float width_section = section->getBgWidth();
	float height_section = section->getBgHeight();
	CGame* game = CGame::GetInstance();

	cx = camBoxLeft + 16 * 2;
	cy = camBoxBottom - 16 * 3;
	cx -= game->GetScreenWidth() / 2 - 8;
	cy -= game->GetScreenHeight() / 2 - 16;

	if (game->GetState() == GameState::PLAY_OVERHEAD)
		cy += 18;
}

void GameScene::MoveCameraBeforeSwitchSection(float& cx, float& cy)
{
	CGame* game = CGame::GetInstance();

	if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_RIGHT ||
		CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_RIGHT_JASON)
	{
		game->GetCamPos(cx, cy);
		if ((int)round(cy) % 256 > 16) {
			cy--;
		}
		else if ((int)round(cy) % 256 < 16) {
			cy++;
		}
		else {
			if (cx + 2 <= Sections[CurrentSectionId]->getBgWidth())
				cx += 2;
			else cx = Sections[CurrentSectionId]->getBgWidth();
		}
	}

	if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_LEFT ||
		CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_LEFT_JASON)
	{
		game->GetCamPos(cx, cy);
		if ((int)cy % 256 > 16) {
			cy--;
		}
		else if ((int)cy % 256 < 16) {
			cy++;
		}
		else {
			if (cx - 2 + game->GetScreenWidth() >= 0)
				cx -= 2;
			else cx = -game->GetScreenWidth();
		}
	}

	//OverHead
	GameState _gameState = game->GetState();
	if (_gameState == GameState::SECTION_SWITCH_OVERHEAD_LEFT)
	{
		if (cx - 2 + game->GetScreenWidth() >= 0)
			cx -= 2;
		else cx = -game->GetScreenWidth();
	}

	if (_gameState == GameState::SECTION_SWITCH_OVERHEAD_RIGHT)
	{
		game->GetCamPos(cx, cy);
		if ((int)round(cy) % 256 > 0 && (int)round(cy) % 256 < 100)
		{
			cy--;
		}
		else
			if ((int)round(cy) % 256 > 0 && (int)round(cy) % 256 > 100)
			{
				cy++;
			}
			else
				if (cx + 2 <= Sections[CurrentSectionId]->getBgWidth())
				{
					cx += 2;
				}
				else cx = Sections[CurrentSectionId]->getBgWidth();
	}

	if (_gameState == GameState::SECTION_SWITCH_OVERHEAD_UP)
	{
		if (cy - 2 + game->GetScreenHeight() + 32 >= 0)
			cy -= 2;
		else cy = -game->GetScreenHeight() - 32;
	}

	if (_gameState == GameState::SECTION_SWITCH_OVERHEAD_DOWN)
	{
		if (cy + 2 <= Sections[CurrentSectionId]->getBgHeight())
			cy += 2;
		else cy = Sections[CurrentSectionId]->getBgHeight();
	}
}

void GameScene::PreventCameraOverBoundingBox(float& cx, float& cy)
{
	LPSECTION section = this->GetCurrentSection();
	float width_section = section->getBgWidth();
	float height_section = section->getBgHeight();
	CGame* game = CGame::GetInstance();
	GameState _gameState = game->GetState();

	if (_gameState != GameState::SECTION_SWITCH_RIGHT &&
		_gameState != GameState::SECTION_SWITCH_RIGHT_JASON &&
		_gameState != GameState::SECTION_SWITCH_OVERHEAD_RIGHT)
		if (cx + game->GetScreenWidth() > width_section)
			cx = width_section - game->GetScreenWidth();

	if (_gameState != GameState::SECTION_SWITCH_OVERHEAD_DOWN)
		if (cy + game->GetScreenHeight() > height_section)
		{
			cy = height_section - game->GetScreenHeight();
		}

	if (_gameState != GameState::SECTION_SWITCH_LEFT &&
		_gameState != GameState::SECTION_SWITCH_LEFT_JASON &&
		_gameState != GameState::SECTION_SWITCH_OVERHEAD_LEFT)
		if (cx < 0) cx = 0;

	if (_gameState != GameState::SECTION_SWITCH_OVERHEAD_UP)
		if (cy < 0) cy = 0;
}

void GameScene::ResetGameStateAfterSwichtSection()
{
	float cx, cy;
	LPSECTION section = this->GetCurrentSection();
	float width_section = section->getBgWidth();
	float height_section = section->getBgHeight();
	CGame* game = CGame::GetInstance();
	game->GetCamPos(cx, cy);

	float x_toPortal, y_toPortal, x_fromPortal, y_fromPortal;
	toPortal->GetPosition(x_toPortal, y_toPortal);
	fromPortal->GetPosition(x_fromPortal, y_fromPortal);

	//Save game when over portal
	CGameManager* manager = CGameManager::GetInstance();
}
