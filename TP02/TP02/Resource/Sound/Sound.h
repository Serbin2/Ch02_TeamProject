// [작성자] : 박재현 
// [DESC] : 사운드 

#pragma once
#include "../ResourceBase.h"
enum class SoundType
{
	BGM,
	Normal,
};

class CSound : public CResourceBase
{
public:
	CSound();
	virtual ~CSound();

	bool LoadWave(fs::path fullPath);
	void Play(bool loop = false);
	void Stop(bool reset = false);

private:
	virtual void LoadFile(const std::wstring& path) {}
	virtual void SaveFile(const std::wstring& path) {}

private:
	LPDIRECTSOUNDBUFFER m_SoundBuffer = nullptr;
	DSBUFFERDESC m_BufferDesc = {};
};
