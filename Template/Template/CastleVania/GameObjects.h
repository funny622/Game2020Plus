#pragma once

#include <Windows.h>
#include <vector>
#include <d3dx9.h>
#include <algorithm>

#include "Utils.h"
#include "GTexture.h"
#include "Game.h"

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

struct CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;

class GameObject
{
protected:
	float x, y;
	float vx, vy;
	int state = 0;
public:

	int currentSectionId = 0;
	int classId = -1;
	int objectId = -1;
	int zIndex = ZINDEX_DEFAULT;
	bool isHiddenByForeground = true;
	bool isUpdatedWhenOffScreen = false;

	GameObject();

	void SetPosition(float x, float y) { this->x = x, this->y = y; };
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }

	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }

	void SetState(int state) { this->state = state; }
	int GetState() { return this->state; }

	void RenderBoundingBox();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;

	void Update(DWORD dt, vector<GameObject*>* coObjects);

	virtual void Render(float offsetX, float offsetY) = 0;

	~GameObject();
};

typedef GameObject* LPGAMEOBJECT;
#pragma endregion


#pragma region CCollisionEvent
/// <summary>
/// <para> Stores infomation of a collision event from ONE game object to ANOTHER </para>
/// <para> Based on Swept AABB algorithm </para>
/// </summary>
struct CCollisionEvent
{
	LPGAMEOBJECT sourceObject;
	LPGAMEOBJECT otherObject;

	/// <summary>
	/// <para> Predicted time until colision based on swept AABB Algoithm </para>
	/// <para> Collision on next frame only when: 0&lt;timeEntry&lt;1 </para>
	/// </summary>
	float timeEntry;

	/// <summary>
	/// <para> A small distance to move in order not to collide to otherObject </para> 
	/// <para> Backward of (dx,dy) </para>
	/// <para> Value can only be either -1, 0 or 1 </para>
	/// </summary>
	float nx, ny;

	/// <summary>
	/// <para> Relative movement from sourceObject to otherObject perspective (i.e otherObject is static) </para>
	/// </summary>
	float rdx, rdy;

	CCollisionEvent(LPGAMEOBJECT sourceObject, LPGAMEOBJECT otherObject, float timeEntry, float rdx, float rdy, float nx, float ny)
	{
		this->sourceObject = sourceObject;
		this->otherObject = otherObject;
		this->timeEntry = timeEntry;
		this->rdx = rdx;
		this->rdy = rdy;
		this->nx = nx;
		this->ny = ny;
	}

	/// <summary>
	/// <para> One doesn't have to go to this function definition just to know it compares 2 entryTime, do they? </para>
	/// </summary>
	/// <returns>returns TRUE iff a.timeEntry&lt;b.timeEntry</returns>
	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->timeEntry < b->timeEntry;
	}
};
#pragma endregion

