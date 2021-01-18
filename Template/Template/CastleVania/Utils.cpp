#include "Utils.h"
#include "Game.h"
#include "GameObjects.h"

void DebugOut(wchar_t* fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	wchar_t dbg_out[4096];
	vswprintf_s(dbg_out, fmt, argp);
	va_end(argp);
	OutputDebugString(dbg_out);
}

vector<string> split(string line, string delimeter)
{
	vector<string> tokens;
	//size_t last = 0; size_t next = 0;
	//while ((next = line.find(delimeter, last)) != string::npos)
	//{
	//	tokens.push_back(line.substr(last, next - last));
	//	last = next + 1;
	//}
	//tokens.push_back(line.substr(last));

	string temp = "";
	for (int i = 0; i < line.length(); i++)
		if (line[i] == ' ' || line[i] == '\t')
			temp += ' ';
		else temp += line[i];

	line = temp + " ";
	int i = 0;
	string sub = " ";
	while (i < line.length())
	{
		//Luoc bo cac khoang trang
		if (line[i] == ' ')
		{
			if (sub[0] != ' ')
				tokens.push_back(sub);
			sub = " ";
			i++;
			continue;
		}
		if (sub == " ") sub = line[i];
		else
			sub += line[i];
		i++;
	}
	return tokens;
}

/*
char * string to wchar_t* string.
*/
wstring ToWSTR(string st)
{
	const char* str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t* wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring wstr(wcstring);

	// delete wcstring   // << can I ? 
	return wstr;
}

/*
	Convert char* string to wchar_t* string.
*/
LPCWSTR ToLPCWSTR(string st)
{
	const char* str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t* wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring* w = new wstring(wcstring);

	// delete wcstring   // << can I ? 
	return w->c_str();
}

int IsKeyDown(int KeyCode)
{
	auto game = CGame::GetInstance();
	return game->IsKeyDown(KeyCode);
}

bool checkObjInCamera(CGameObject* obj, float extendOffset)
{
	return true;
}

float RandomFloat(float a, float b)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}
