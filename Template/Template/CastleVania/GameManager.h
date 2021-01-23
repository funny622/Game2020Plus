#pragma once

#include "Enemy.h"
#include <string>
#include <windows.h>
#include "Timer.h"
#include <string>
#include "GameObjects.h"
#include "Animations.h"
using namespace std;

#define MAX_HEALTH_SOPHIA 80
#define MAX_HEALTH_JASONSIDEVIEW 80
#define MAX_HEALTH_JASONOVERHEAD 80
#define MAX_GUN_LEVEL 8
#define BODY_DAMAGE_ENEMY 5 * 1
#define BODY_DAMAGE_BULLET 5 * 1
#define BODY_DAMAGE_LAVA 20 * 1
#define BODY_DAMAGE_SPIKE 20 * 1
#define BODY_DAMAGE_FALL 80

#define MAX_AMMUNITIONS 99
#define ID_SECTION_BOSS 1000959

enum class Items {
	PowerGrey, // add 1 bar
	PowerFlashingRed, // add 4 bars
	HoverGrey,
	HoverFlashingRed,
	GunGrey,
	GunFlashingRed,
	HomingMissile, // item weapon
	MultiwarheadMissile,// item weapon
	ThunderBreak,// item weapon
	HyperBeam, //item to destroy the guardian that stands between you and Area 2.
};

enum class TypeWeapons
{
	HomingMissile,
	MultiwarheadMissile,
	ThunderBreak,
};

class CGameManager : public TimeTracker
{
private:
	CGameManager();
	static CGameManager* _instance;
	const int BOSS_EFFECT_DURATION = 2000;
	const int BOSS_EFFECT_FADE_IN_DURATION = 500;
	int left = 3;
	int flagPlayer = 1; // 1: Sophia 2:Jason SideView 3:Jason OverHead
	bool Saved = false;
	int healthSophia;
	int healthJason;

	void SupportRenderHeath(int health);
	int SupportGetIDNumber(int number);
	void SupportAnalysNumber(int number, int& first, int& second);

	LPTIMER effectBossFlashingTimer;
	LPTIMER effectBossFadeInTimer;

	CObjectAnimationHanlders WeaponMenu;

	CObjectAnimationHanlders EffectFaded;
	CObjectAnimationHanlders EffectFadedIn;
	CObjectAnimationHanlders EffectFadedOut;

	CObjectAnimationHanlders SelectedLeft;
	CObjectAnimationHanlders SelectedRight;

	bool isWeaponMenuActive = false;
	int idSelectedItem = 1;

public:
	static CGameManager* GetInstance();

	int getLeft() { return this->left; }
	void subLeft();

	//event method
	void initEffectFaded();

	virtual void HandleTimerTick(LPTIMER sender);

	//Effect
	bool isEffectFaded = false;
	bool isEffectBoss = false;
	bool isEffectBossFadeIn = false;

	int getPlayer() { return this->flagPlayer; }

	void resetHealth();

	//Save game
	bool isOverheadtoSideView = false;
	bool isSaved() { return this->Saved; }

	void Update(DWORD dt);
	void UpdateEffect(DWORD dt);

	void RenderHealth();
	void RenderHealthGun();
	void RenderWeapon();
	void RenderEffect();

	TypeWeapons selectedWeapon = TypeWeapons::MultiwarheadMissile;
	void OpenMenuWeapon();
	void CloseMenuWeapon();
	bool isMenuWeaponOpen() { return this->isWeaponMenuActive; }

	void NextSelectedItem();
	void BackSelectedItem();

};


//INFO LEVEL GUN POWER
//1	Jason fires shots that stop halfway across the screen.
//2	Jason can fire two shots that travel all the way across the screen.
//3	Jason can fire three shots that travel all the way across the screen.
//4	Jason can fire three shots that travel all the way across the screen.
//5	Some bullets that Jason fires will fly off in slowly expanding circles to either side.
//6	Some bullets that Jason fires will fly off in quickly expanding circles to either side.
//7	Jason can fire up to five bullets that travel in slowly expanding waves
//8	Jason can fire a stream of bullets that can pass through solid objects, and travel in quickly expanding waves.

//Also note that Jason can recover all of his power by returning to SOPHIA