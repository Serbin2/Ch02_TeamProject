#pragma once

#include "../Standard.h"

using namespace std;


//	ui를 위한 객체입니다.
class CInterface
{
public:
	CInterface();
	~CInterface();

	bool AddUI(int index, string name);
	bool SetValue(int index, int value);
	bool SetValue(int index, float value);
	bool SetValue(int index, string value);
	bool RemoveUI(int index);
	void SetUIColor(int color);

private:

	void EraseUI(int index);

	//	UI 출력을 위한 멤버
	HANDLE m_hOP;
	

	string m_sUIs[WORLD_SIZE];
	int m_iUIColor[WORLD_SIZE];
	bool m_bAccupied[WORLD_SIZE] = { false, };
};

//	SetConsoleCursorPosition(m_hOP, cdPos);
//	SetConsoleTextAttribute(m_hOP, m_aBuffer[y][x].color);