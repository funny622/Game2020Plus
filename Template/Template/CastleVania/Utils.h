#pragma once

#include <Windows.h>

#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <string>

#include "SideviewConst.h"
#include "OverheadConst.h"
#include "ZIndexConst.h"
#include "CustomKeyEvent.h"


using namespace std;

class GameObject;

void DebugOut(wchar_t* fmt, ...);
float RandomFloat(float a, float b);

vector<string> split(string line, string delimeter = "\t");
wstring ToWSTR(string st);

LPCWSTR ToLPCWSTR(string st);



int IsKeyDown(int KeyCode);


std::vector<CCustomKeyEvent*> NewKeyEvents();

const int SCREEN_EXTEND_OFFSET_DEFAULT = 4;
bool checkObjInCamera(GameObject* obj, float extendOffset = SCREEN_EXTEND_OFFSET_DEFAULT);

typedef tuple<int, int, int> Color;
