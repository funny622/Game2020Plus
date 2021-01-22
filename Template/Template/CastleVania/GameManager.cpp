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

