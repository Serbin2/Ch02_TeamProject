#include "../ResourceManager/ResourceManager.h"
#include "../../Resource/Sound/Sound.h"

void CResourceManager::Init(HWND hHwnd)
{
	m_hHwnd = hHwnd;
	m_ResourcePath = std::filesystem::current_path().parent_path() / "GameResource";
}

void CResourceManager::Clear()
{
	m_ResourcePath.clear();
	m_pSounds.clear();
}

std::shared_ptr<CSound> CResourceManager::GetSound(const std::wstring& key)
{
	auto it = m_pSounds.find(key);
	if (it != m_pSounds.end())
		return it->second;

	return nullptr;
}

std::shared_ptr<CSound>  CResourceManager::LoadSound(const std::wstring& key, const std::wstring& path, ESoundType SoundType)
{
	// 이미 존재하는 경우 
	if (m_pSounds.find(key) != m_pSounds.end())
	{
		return m_pSounds[key];
	}

	std::string Folder;

	switch (SoundType)
	{
	case ESoundType::BGM:
		Folder = "BGM";
		break;
	case ESoundType::SFX:
		Folder = "SFX";
		break;
	}

	fs::path Path = m_ResourcePath / "Sound" / Folder / path;

	std::shared_ptr<CSound> pSound = std::make_shared<CSound>();
	pSound->LoadWave(Path);
	m_pSounds[key] = pSound;

	return pSound;
}
