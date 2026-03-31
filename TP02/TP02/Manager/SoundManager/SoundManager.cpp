#include "../SoundManager/SoundManager.h"
#include "../ResourceManager/ResourceManager.h"

CSoundManager::~CSoundManager()
{
	if (m_SoundDevice)
		m_SoundDevice->Release();

	if (m_pBGM)
	{
		m_pBGM->Stop(true);
		m_pBGM = nullptr;	
	}
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

void CSoundManager::PlaySFX(const std::wstring& wsKey)
{
	auto pSound = GET_SINGLE(CResourceManager)->GetSound(wsKey);
	if (pSound == nullptr)
	{
		return;
	}

	pSound->Play(false);
}

void CSoundManager::PlayBGM(const std::wstring& wsKey)
{
	if (m_pBGM)
	{
		m_pBGM->Stop(true);
		m_pBGM = nullptr;	
	}

	m_pBGM = GET_SINGLE(CResourceManager)->GetSound(wsKey);
	if (m_pBGM == nullptr)
	{
		return;
	}

	m_pBGM->Play(true);
}

void CSoundManager::RePlayBGM()
{
	if (!m_pBGM)
	{
		return;
	}

	m_pBGM->Play(true);
}

void CSoundManager::StopBGM()
{
	if (!m_pBGM)
	{
		return;
	}

	m_pBGM->Stop();
}
