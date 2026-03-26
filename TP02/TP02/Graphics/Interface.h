#pragma once

#include "../Standard.h"

using namespace std;


//	ui를 위한 객체입니다.
class CInterface
{
public:
	CInterface();
	~CInterface();

	//	UI를 추가합니다.
	//	이미 사용중인 UI가 있으면 실패합니다.
	bool AddUI(int index, string name);

	//	UI에 값을 그려줍니다.
	//	사용중인 ui가 아니면 실패합니다.
	bool SetValue(int index, int value);
	bool SetValue(int index, float value);
	bool SetValue(int index, string value);

	//	ui를 제거합니다.
	//	사용중인 ui가 아니면 실패합니다.
	bool RemoveUI(int index);

	//	ui 색상을 변경합니다.
	//	주의 : ui를 다시 그려주지 않습니다. name까지 바꾸려면 AddUI 이전에 호출하세요
	void SetUIColor(int index, int color) { m_iUIColor[index] = color; };

private:

	void Redraw(int index);

	//	UI 출력을 위한 멤버
	HANDLE m_hOP;
	
	string m_sblank;
	string m_sUIs[WORLD_SIZE];
	int m_iUIColor[WORLD_SIZE];
	bool m_bAccupied[WORLD_SIZE] = { false, };
};

//	SetConsoleCursorPosition(m_hOP, cdPos);
//	SetConsoleTextAttribute(m_hOP, m_aBuffer[y][x].color);