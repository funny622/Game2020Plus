#pragma once
#include "GameObjects.h"

class CTileArea :
    public CGameObject
{
protected:
    int width;
    int height;

    // Use class id to know the exact tile type

public:
    void SetSize(int width, int height);
    void GetSize(int& width, int& height);

    CTileArea() {}
    CTileArea(int classId, int x, int y, int width, int height, int sectionId);

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs) {};
    virtual void Render(float offsetX, float offsetY);
};

typedef CTileArea* LPTILE_AREA;
