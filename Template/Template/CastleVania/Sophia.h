#pragma once
#include "AnimatableObject.h"
#include <queue>
#include <math.h>
#include "Timer.h"

// 8 23 31 39

const int SOPHIA_BOUNDBOX_WIDTH = 21;
const int SOPHIA_BOUNDBOX_HEIGHT = 15;
const int SOPHIA_BOUNDBOX_OFFSETX = 10;
const int SOPHIA_BOUNDBOX_OFFSETY = 23;

const int SOPHIA_SPRITE_WIDTH = 40;
const int SOPHIA_SPRITE_HEIGHT = 40;

const float SOPHIA_MAX_SPEED = 0.1;
const float SOPHIA_ENGINE = 0.0005;
const float FRICTION = 0.00025;

const float SOPHIA_GRAVITY = 0.0005f;
const float SOPHIA_MAX_FALL_SPEED = 0.175f;
const float SOPHIA_JUMP_FORCE = 0.26f;

const float SOPHIA_AX = 0.0001;

const int INVULNERABLE_DURATION = 450;

/// <summary>
/// SOPHIA is also animatable, however, it has a completely different kind of animation system
/// so we have to somehow override the render method
/// </summary>
class CSophia : public CAnimatableObject, public TimeTracker
{
private:
    const int DYING_EFFECT_DURATION = 1700;
    static CSophia* __instance;
    int directionState, gunState, bodyState, wheelState;
public:
    static CSophia* GetInstance();
    virtual void HandleTimerTick(LPTIMER sender);
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

    ~CSophia();
};

typedef CSophia* LPSOPHIA;
