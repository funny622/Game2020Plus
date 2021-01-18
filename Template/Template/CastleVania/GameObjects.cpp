#include "GameObjects.h"

GameObject::GameObject()
{
	x = y = 0;
	vx = vy = 0;
}

void GameObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}


void GameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = GTexture::GetInstance()->Get(0);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	// CuteTN to do: work on this function later... or maybe never :)
	// CGame::GetInstance()->Draw(x, y, bbox, rect.left, rect.top, rect.right, rect.bottom, 32);
}

GameObject::~GameObject()
{

}
