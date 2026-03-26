#include "Input.h"
#include "../Standard.h"

CInput::CInput()
{

}

CInput::~CInput()
{

}

CInput* CInput::m_pInstance = nullptr;

CInput* CInput::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CInput();
	}
	return m_pInstance;
}

void CInput::Release()
{
	if (m_pInstance == nullptr)	return;

	delete m_pInstance;
	m_pInstance = nullptr;
}

void CInput::Update()
{
	memcpy(m_aPrev, m_aCurr, sizeof(m_aPrev));
	for (int i = 0; i < 256; i++)
	{
		m_aCurr[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
	}
}

bool CInput::IsKeyDown(int key)
{
	return m_aCurr[key] && !m_aPrev[key];
}

bool CInput::IsKeyUp(int key)
{
	return m_aPrev[key] && !m_aCurr[key];
}

bool CInput::IsKeyPressed(int key)
{
	return m_aCurr[key];
}