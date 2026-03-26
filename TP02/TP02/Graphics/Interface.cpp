#include "Interface.h"
#include <format>

CInterface::CInterface()
{
	m_hOP = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < WORLD_SIZE; i++)
	{	//	기본 색상 지정
		m_iUIColor[i] = TEXT_FOREGROUND_WHITE | TEXT_BACKGROUND_BLACK;
	}

	m_sblank = "                    ";
}

CInterface::~CInterface()
{

}

//	UI를  추가합니다.
bool CInterface::AddUI(int index, string name)
{
	//	이미 사용중인 자리입니다.
	if (m_bAccupied[index])		return false;

	//	UI 사용
	m_bAccupied[index] = true;
	m_sUIs[index] = name;

	//	출력합니다
	COORD pos;
	pos.X = UI_POS_X;
	pos.Y = index + 1;
	SetConsoleCursorPosition(m_hOP, pos);
	SetConsoleTextAttribute(m_hOP, m_iUIColor[index]);
	cout << name;

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

	size_t size = m_sUIs[index].length();
	size_t valSize = value.length();
	int blank = 20 - (int)size - (int)valSize;
	string str = value + m_sblank;
	
	if (blank < 0)	blank = 0;
	//	출력합니다
	string str2 = str.substr(0, blank);
	COORD pos;
	pos.X = UI_POS_X + size;
	pos.Y = index + 1;
	SetConsoleCursorPosition(m_hOP, pos);
	SetConsoleTextAttribute(m_hOP, m_iUIColor[index]);
	cout << str2;

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
	cout << m_sblank;

	m_bAccupied[index] = false;
	return true;
}

void CInterface::Redraw(int index)
{
	COORD pos;
	pos.X = UI_POS_X;
	pos.Y = index + 1;
	SetConsoleCursorPosition(m_hOP, pos);
	int color = TEXT_BACKGROUND_BLACK;
	SetConsoleTextAttribute(m_hOP, color);
	cout << m_sblank;
	SetConsoleCursorPosition(m_hOP, pos);
	SetConsoleTextAttribute(m_hOP, m_iUIColor[index]);
	cout << m_sUIs[index];
}