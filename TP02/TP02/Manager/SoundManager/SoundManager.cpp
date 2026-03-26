#include "../SoundManager/SoundManager.h"
#include "../ResourceManager/ResourceManager.h"
#include "../../Resource/Sound/Sound.h"

CSoundManager::~CSoundManager()
{
	if (m_SoundDevice)
		m_SoundDevice->Release();
}

void CSoundManager::Init(HWND hHwnd)
{
	if (FAILED(::DirectSoundCreate(NULL, &m_SoundDevice, NULL)))
	{
		::MessageBox(NULL, L"사운드 디바이스 초기화", L"SYSTEM ERROR", MB_OK);
		return;
	}

	if (FAILED(m_SoundDevice->SetCooperativeLevel(hHwnd, DSSCL_PRIORITY)))
	{
		::MessageBox(NULL, L"사운드 디바이스 초기화", L"SYSTEM ERROR", MB_OK);
		return;
	}
}

void CSoundManager::Play(const std::wstring& wsKey, bool bLoop /*= false*/)
{
	auto pSound = GET_SINGLE(CResourceManager)->GetSound(wsKey);
	if (pSound == nullptr)
	{
		return;
	}

	pSound->Play(bLoop);
}
