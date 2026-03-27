#pragma once
#include "../../Utils/Common.h"


class CSoundManager
{
	DECLARE_SINGLE(CSoundManager)
	~CSoundManager();

public:
	void Init(HWND hHwnd);

	void Play(const std::wstring& wsKey, bool bLoop = false);

	LPDIRECTSOUND GetSoundDevice() { return m_SoundDevice; }

private:
	LPDIRECTSOUND m_SoundDevice = nullptr;
};

