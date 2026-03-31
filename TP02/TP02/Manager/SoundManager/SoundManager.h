#pragma once
#include "../../Utils/Common.h"
#include "../../Resource/Sound/Sound.h"


class CSoundManager
{
	DECLARE_SINGLE(CSoundManager)
	~CSoundManager();

public:
	void Init(HWND hHwnd);

	// 사운드 이펙트 - 반복 x 
	void PlaySFX(const std::wstring& wsKey);
	// BGM - 반복 o
	void PlayBGM(const std::wstring& wsKey);
	// 기존에 재생하던 브금 존재시 이어서 재생
	void RePlayBGM();
	// 재생 멈추기
	void StopBGM();

	LPDIRECTSOUND GetSoundDevice() { return m_SoundDevice; }

private:
	LPDIRECTSOUND m_SoundDevice = nullptr;

	std::shared_ptr<CSound> m_pBGM;
};

