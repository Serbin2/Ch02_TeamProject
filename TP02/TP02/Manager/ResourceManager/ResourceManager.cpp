#include "../ResourceManager/ResourceManager.h"
#include "../../Resource/Sound/Sound.h"

void CResourceManager::Init(HWND hHwnd)
{
	m_hHwnd = hHwnd;
	m_ResourcePath = std::filesystem::current_path().parent_path() / "GameResource";
}

void CResourceManager::Clear()
{
	m_pSounds.clear();
}

std::shared_ptr<CSound> CResourceManager::GetSound(const std::wstring& key)
{
	return m_pSounds[key]; 
}

std::shared_ptr<CSound>  CResourceManager::LoadSound(const std::wstring& key, const std::wstring& path)
{
	// 이미 존재하는 경우 
	if (m_pSounds.find(key) != m_pSounds.end())
	{
		return m_pSounds[key];
	}

	fs::path Path = m_ResourcePath / "Sound" / path;

	std::shared_ptr<CSound> pSound = std::make_shared<CSound>();
	pSound->LoadWave(Path);
	m_pSounds[key] = pSound;

	return pSound;
}
