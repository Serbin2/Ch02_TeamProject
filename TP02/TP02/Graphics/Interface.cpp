#include "Interface.h"
#include <format>

CInterface::CInterface()
{
	m_hOP = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < WORLD_SIZE; i++)
	{	//	기본 색상 지정
		m_iUIColor[i] = TEXT_FOREGROUND_WHITE | TEXT_BACKGROUND_BLACK;
	}

	m_sBlank = "                    ";
}

CInterface::~CInterface()
{

}

CInterface* CInterface::m_pInstance = nullptr;

CInterface* CInterface::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CInterface();
	}

	return m_pInstance;
}

void CInterface::Release()
{
	if (m_pInstance == nullptr)	return;

	delete m_pInstance;
	m_pInstance = nullptr;
}

//	UI를  추가합니다.
bool CInterface::AddUI(int index, string name)
{
	//	이미 사용중인 자리입니다.
	if (m_bAccupied[index])		return false;

	//	UI 사용
	m_bAccupied[index] = true;
	m_sName[index] = name;

	return true;
}

bool CInterface::SetValue(int index, int value)
{
	return SetValue(index, to_string(value));
}

bool CInterface::SetValue(int index, float value)
{
	return SetValue(index, format("{:.2f}", value));
}

bool CInterface::SetValue(int index, string value)
{
	if (!m_bAccupied[index])	return false;

	string str = m_sName[index] + value + m_sBlank;
	m_sUIs[index] = str.substr(0, 20);
	//	출력합니다
	RenderUI(index);
	return true;
}

bool CInterface::RemoveUI(int index)
{
	if (!m_bAccupied[index])	return false;

	COORD pos;
	pos.X = UI_POS_X;
	pos.Y = index + 1;
	SetConsoleCursorPosition(m_hOP, pos);
	int color = TEXT_BACKGROUND_BLACK;
	SetConsoleTextAttribute(m_hOP, color);
	cout << m_sBlank;

	m_bAccupied[index] = false;
	return true;
}

void CInterface::RenderUI(int index)
{
	COORD pos;
	pos.X = UI_POS_X;
	pos.Y = index + 1;
	SetConsoleCursorPosition(m_hOP, pos);
	SetConsoleTextAttribute(m_hOP, m_iUIColor[index]);
	cout << m_sUIs[index];
}

void CInterface::Redraw()
{
	for (int i = 0; i < WORLD_SIZE; i++)
	{
		RenderUI(i);
	}
}