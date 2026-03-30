#pragma once
// [작성자] : 박재현 
// [DESC] : 리소스 매니저 

#include "../../Utils/Common.h"

class CSound;


enum class ESoundType
{
	BGM,
	SFX
};

class CResourceManager
{
	DECLARE_SINGLE(CResourceManager)

public:
	void Init(HWND hHwnd);
	void Clear();

public:
	std::shared_ptr<CSound> GetSound(const std::wstring& key);
	std::shared_ptr<CSound> LoadSound(const std::wstring& key, const std::wstring& path, ESoundType SoundType);

private:
	HWND m_hHwnd;
	std::string mRootPath = "GameResource";
	fs::path m_ResourcePath;

private:
	std::unordered_map<std::wstring, std::shared_ptr<CSound>> m_pSounds;
};

