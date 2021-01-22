#pragma once

#include "Enemy.h"
#include <string>
#include <windows.h>
#include "Timer.h"
#include <string>
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

public:
	static CGameManager* GetInstance();

	int getLeft() { return this->left; }
	void subLeft();

	virtual void HandleTimerTick(LPTIMER sender);

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