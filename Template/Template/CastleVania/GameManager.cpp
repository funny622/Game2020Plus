#include "GameManager.h"
//#include "Sophia.h"
//#include "JasonSideview.h"
//#include "JasonOverhead.h"
#include <fstream>

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_STATE_ANIMATION	5
#define SCENE_OBJECT_ANIMATION	6
#define MAX_SCENE_LINE 1024


#define HEALTH 9041
#define HEALTH0 9031
#define HEALTH1 9032
#define HEALTH2 9033
#define HEALTH3 9034
#define HEALTH4 9035
#define HEALTH5 9036
#define HEALTH6 9037
#define HEALTH7 9038
#define HEALTH8 9039

#define GUN0 11111


#define X_HEALTH 20
#define Y_HEALTH 120
#define X_GUN 20
#define Y_GUN 80

//Weapon
#define ID_WEAPONMENU 11111
#define STATE_WEAPON 99999
#define ID_NUMBER 22222
#define ID_SELECTION_LEFT 33333
#define ID_SELECTION_RIGHT 44444
#define STATE_SELECTION_LEFT 11010
#define STATE_SELECTION_RIGHT	11020
#define ID_GUN 55555
#define ID_HEALTHOVERHEAD 66666
#define ID_EFFECT 77777
#define ID_FADE_IN 88888
#define ID_FADE_OUT 99999
#define ID_STATE_EFFECT 12000
#define ID_STATE_EFFECT_FADE_IN 13000

#define HEALTHOH0 9431

#define NUMBER_0	11000
#define NUMBER_1	11001
#define NUMBER_2	11002
#define NUMBER_3	11003
#define NUMBER_4	11004
#define NUMBER_5	11005
#define NUMBER_6	11006
#define NUMBER_7	11007
#define NUMBER_8	11008
#define NUMBER_9	11009

#define POS_Y_NUMBER 190
#define POS_X_NUMBER1	86
#define POS_X_NUMBER2	96
#define POS_X_NUMBER3	118
#define POS_X_NUMBER4	128
#define POS_X_NUMBER5	150
#define POS_X_NUMBER6	160

#define POS_X_SELECTED1	72
#define POS_X_SELECTED2	104
#define POS_X_SELECTED3	136
#define POS_Y_SELECTED	173

CGameManager* CGameManager::_instance = nullptr;

CGameManager::CGameManager() {
}

CGameManager* CGameManager::GetInstance()
{
	if (_instance == nullptr) _instance = new CGameManager();
	return _instance;
}

void CGameManager::subLeft()
{
	if (left == -1) return;
	left--;
}

void CGameManager::HandleTimerTick(LPTIMER sender)
{
}

void CGameManager::initEffectFaded()
{
	this->isEffectFaded = true;
}

void CGameManager::SupportRenderHeath(int health)
{
	int flag_StateHealth = HEALTH0;
	if (health > 0 && health <= 10)
		flag_StateHealth = HEALTH1;
	if (health > 10 && health <= 20)
		flag_StateHealth = HEALTH2;
	if (health > 20 && health <= 30)
		flag_StateHealth = HEALTH3;
	if (health > 30 && health <= 40)
		flag_StateHealth = HEALTH4;
	if (health > 40 && health <= 50)
		flag_StateHealth = HEALTH5;
	if (health > 50 && health <= 60)
		flag_StateHealth = HEALTH6;
	if (health > 60 && health <= 70)
		flag_StateHealth = HEALTH7;
	if (health > 70)
		flag_StateHealth = HEALTH8;

	float X_cam, Y_cam;
	CGame::GetInstance()->GetCamPos(X_cam, Y_cam);
}
void CGameManager::SupportAnalysNumber(int number, int& first, int& second)
{
	if (number >= 100)
	{
		first = 9;
		second = 9;
		return;
	}

	if (number <= 0)
	{
		first = 0;
		second = 0;
	}

	first = number / 10;
	second = number % 10;
}

void CGameManager::RenderHealth()
{
	int ID_Player = CGame::GetInstance()->GetCurrentPlayer()->classId;
	switch (ID_Player)
	{
	case CLASS_SOPHIA:
		SupportRenderHeath(healthSophia);
		break;
	case CLASS_JASONSIDEVIEW:
		SupportRenderHeath(healthJason);
		break;
	default:
		SupportRenderHeath(healthJason);
		break;
	}
}
void CGameManager::RenderHealthGun()
{
}
void CGameManager::RenderWeapon()
{
}

void CGameManager::UpdateEffect(DWORD dt)
{
	effectBossFadeInTimer->Update(dt);
	effectBossFlashingTimer->Update(dt);
	if (isEffectFaded)
	{
		EffectFaded[ID_STATE_EFFECT]->Update();
		if (EffectFaded[ID_STATE_EFFECT]->currentFrameIndex == EffectFaded[ID_STATE_EFFECT]->animation->GetNumberOfFrames() - 1)
		{
			isEffectFaded = false;
			EffectFaded[ID_STATE_EFFECT]->currentFrameIndex = 0;
		}
	}
	if (isEffectBossFadeIn)
	{
		EffectFadedIn[ID_STATE_EFFECT_FADE_IN]->Update();
		if (EffectFadedIn[ID_STATE_EFFECT_FADE_IN]->currentFrameIndex == EffectFadedIn[ID_STATE_EFFECT_FADE_IN]->animation->GetNumberOfFrames() - 1)
		{
			isEffectBossFadeIn = false;
			EffectFadedIn[ID_STATE_EFFECT_FADE_IN]->currentFrameIndex = 0;
		}
	}
}

void CGameManager::NextSelectedItem()
{
	if (this->idSelectedItem < 3)
		this->idSelectedItem++;
	else
		this->idSelectedItem = 1;
}

void CGameManager::BackSelectedItem()
{
	if (this->idSelectedItem > 1)
		this->idSelectedItem--;
	else
		this->idSelectedItem = 3;
}

void CGameManager::resetHealth()
{
	this->healthSophia = MAX_HEALTH_SOPHIA;
	this->healthJason = MAX_HEALTH_JASONSIDEVIEW;
}

void CGameManager::Update(DWORD dt)
{
	SelectedLeft[STATE_SELECTION_LEFT]->Update();
	SelectedRight[STATE_SELECTION_RIGHT]->Update();
}

void CGameManager::RenderEffect()
{
	float camx, camy;
	CGame::GetInstance()->GetCamPos(camx, camy);
	if (this->isEffectFaded)
	{
		EffectFaded[ID_STATE_EFFECT]->Render(camx, camy);
	}
	if (this->isEffectBossFadeIn)
	{
		EffectFadedIn[ID_STATE_EFFECT_FADE_IN]->Render(camx, camy);
	}
}

void CGameManager::OpenMenuWeapon()
{
	//Update weapon selected
	switch (this->selectedWeapon)
	{
	case TypeWeapons::HomingMissile:
		idSelectedItem = 1;
		break;
	case TypeWeapons::ThunderBreak:
		idSelectedItem = 2;
		break;
	case TypeWeapons::MultiwarheadMissile:
		idSelectedItem = 3;
		break;
	default:
		break;
	}

	this->isWeaponMenuActive = true;
}

void CGameManager::CloseMenuWeapon()
{
	this->isWeaponMenuActive = false;

	//Update weapon selected
	switch (idSelectedItem)
	{
	case 1:
		this->selectedWeapon = TypeWeapons::HomingMissile;
		break;
	case 2:
		this->selectedWeapon = TypeWeapons::ThunderBreak;
		break;
	case 3:
		this->selectedWeapon = TypeWeapons::MultiwarheadMissile;
		break;
	default:
		this->selectedWeapon = TypeWeapons::HomingMissile;
		break;
	}
}