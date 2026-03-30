#include "Sound.h"
#include "../../Manager/SoundManager/SoundManager.h"

CSound::CSound()
{

}

CSound::~CSound()
{
	if (m_SoundBuffer)
		m_SoundBuffer->Release();
}

bool CSound::LoadWave(fs::path fullPath)
{
	if (fullPath.extension() != L".wav")
		return false;

	HMMIO file = ::mmioOpen((WCHAR*)fullPath.c_str(), NULL, MMIO_READ);
	if (file == 0)
	{
		::MessageBox(NULL, L".wav 로드 실패", L"", MB_OK);
		return false;
	}

	MMCKINFO parent;
	::memset(&parent, 0, sizeof(parent));
	parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	::mmioDescend(file, &parent, NULL, MMIO_FINDRIFF);

	MMCKINFO child;
	::memset(&child, 0, sizeof(child));
	child.ckid = mmioFOURCC('f', 'm', 't', ' ');
	::mmioDescend(file, &child, &parent, MMIO_FINDCHUNK);

	WAVEFORMATEX wft;
	::memset(&wft, 0, sizeof(wft));
	::mmioRead(file, (char*)&wft, sizeof(wft));

	::mmioAscend(file, &child, 0);
	child.ckid = mmioFOURCC('d', 'a', 't', 'a');
	::mmioDescend(file, &child, &parent, MMIO_FINDCHUNK);

	::memset(&m_BufferDesc, 0, sizeof(DSBUFFERDESC));
	m_BufferDesc.dwBufferBytes = child.cksize;
	m_BufferDesc.dwSize = sizeof(DSBUFFERDESC);
	m_BufferDesc.dwFlags = DSBCAPS_STATIC;
	m_BufferDesc.lpwfxFormat = &wft;

	if (FAILED(GET_SINGLE(CSoundManager)->GetSoundDevice()->CreateSoundBuffer(&m_BufferDesc, &m_SoundBuffer, NULL)))
	{
		::MessageBox(NULL, L"사운드 버퍼 생성 실패", L"", MB_OK);
		return false;
	}

	void* write1 = NULL;
	void* write2 = NULL;
	DWORD len1;
	DWORD len2;

	m_SoundBuffer->Lock(0, child.cksize, &write1, &len1, &write2, &len2, 0);

	if (write1)
		::mmioRead(file, (char*)write1, len1);

	if (write2)
		::mmioRead(file, (char*)write2, len2);

	m_SoundBuffer->Unlock(write1, len1, write2, len2);

	::mmioClose(file, 0);
	return true;
}

void CSound::Play(bool loop, bool resume)
{
	// resume이 false일 때만 처음(0)으로 되돌립니다.
	if (!resume)
	{
		m_SoundBuffer->SetCurrentPosition(0);
	}

	if (loop)
		m_SoundBuffer->Play(0, 0, DSBPLAY_LOOPING);
	else
		m_SoundBuffer->Play(0, 0, 0);
}

void CSound::Stop(bool reset)
{
	m_SoundBuffer->Stop();

	if (reset)
		m_SoundBuffer->SetCurrentPosition(0);
}