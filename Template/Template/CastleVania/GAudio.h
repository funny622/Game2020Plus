
#ifndef _DXAUDIO_H
#define _DXAUDIO_H

#include "dsutil.h"
#include "Global.h"
class GAudio
{
public:
	GSoundManager *dsound;

	int Init_DirectSound(HWND);
	GSound *LoadSound(LPTSTR fileName);
	void PlaySound(GSound *);
	void LoopSound(GSound *);
	void StopSound(GSound *);

};
#endif

