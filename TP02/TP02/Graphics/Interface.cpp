#include "Interface.h"


CInterface::CInterface()
{
	m_hOP = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < WORLD_SIZE; i++)
	{	//	기본 색상 지정
		m_iUIColor[i] = TEXT_FOREGROUND_WHITE | TEXT_BACKGROUND_BLACK;
	}
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