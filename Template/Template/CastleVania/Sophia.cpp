#include "Sophia.h"

CSophia* CSophia::__instance = nullptr;

CSophia* CSophia::GetInstance()
{
    if (__instance == nullptr)
    {
        __instance = new CSophia();
    }
    return __instance;
}

void CSophia::HandleTimerTick(LPTIMER sender)
{
}

void CSophia::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    float offsetX = 0, offsetY = 0, width = 0, height = 0;
    bool isTurnedLeft = directionState <= 1;

    left = x + offsetX;
    top = y + offsetY;
    right = left + width;
    bottom = top + height;
}

CSophia::~CSophia()
{
    CAnimatableObject::~CAnimatableObject();
}