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

using namespace std;

class CGameObject;

void DebugOut(wchar_t* fmt, ...);
float RandomFloat(float a, float b);

vector<string> split(string line, string delimeter = "\t");
wstring ToWSTR(string st);

LPCWSTR ToLPCWSTR(string st);

int IsKeyDown(int KeyCode);

const int SCREEN_EXTEND_OFFSET_DEFAULT = 4;
bool checkObjInCamera(CGameObject* obj, float extendOffset = SCREEN_EXTEND_OFFSET_DEFAULT);

typedef tuple<int, int, int> Color;
