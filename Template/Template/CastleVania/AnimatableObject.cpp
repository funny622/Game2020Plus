#include "AnimatableObject.h"

vector<Color> CAnimatableObject::flashingColors =
{
	{255,0,100},
	{10,255,10},
	{255, 255, 255},
	{255,0,100},
	{10,255,10},
	{255, 255, 255},
	{255,0,100},
	{10,255,10},
	{255, 255, 255}
};

CAnimatableObject::CAnimatableObject(int classId, int x, int y, int sectionId, int objAnimsId)
{
}

CAnimatableObject::CAnimatableObject(int classId, int x, int y, int sectionId) : CAnimatableObject::CAnimatableObject(classId, x, y, sectionId, LookUpAnimationsId(classId))
{
}


int CAnimatableObject::LookUpAnimationsId(int classId)
{
	return 0;
}

void CAnimatableObject::UpdatePosition(DWORD dt)
{
	this->x += this->vx * dt;
	this->y += this->vy * dt;
}

void CAnimatableObject::Render(float offsetX, float offsetY)
{
}

void CAnimatableObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	UpdatePosition(dt);
}

CAnimatableObject::~CAnimatableObject()
{
}

