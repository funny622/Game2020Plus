#pragma once

#include <Windows.h>
#include <vector>
#include <d3dx9.h>
#include <algorithm>

#include "Utils.h"
#include "GTexture.h"
#include "Game.h"

using namespace std;

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

	~GameObject();
};

typedef GameObject* LPGAMEOBJECT;
#pragma endregion

