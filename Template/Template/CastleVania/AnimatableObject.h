#pragma once
#include "GameObjects.h"
#include "Animations.h"

class CAnimatableObject :
	public GameObject
{
protected:
	int modifyR = 255;
	int modifyG = 255;
	int modifyB = 255;
	int modifyA = 255;

	static vector<Color> flashingColors;
	int LookUpAnimationsId(int classId);

public:

	bool allowOverlapWithBlocks = false;

	CAnimatableObject() {};
	CAnimatableObject(int classId, int x, int y, int sectionId, int animsId);
	CAnimatableObject(int classId, int x, int y, int sectionId);

	virtual void UpdatePosition(DWORD dt);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

	virtual void Render(float offsetX, float offsetY);

	virtual ~CAnimatableObject();
};

