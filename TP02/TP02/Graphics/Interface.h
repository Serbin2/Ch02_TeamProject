#pragma once

#include "../Standard.h"

using namespace std;


//	ui를 위한 객체입니다.
class CInterface
{
private:
	CInterface();
	~CInterface();

public:

	static CInterface* GetInstance();
	static void Release();

	//	UI를 추가합니다.
	//	이미 사용중인 UI가 있으면 실패합니다.
	bool AddUI(int index, string name);

	//	UI에 값을 설정합니다.
	//	설정 후에 출력까지 해줍니다.
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

	//	UI 전체를 다시 그립니다
	//	배경은 지워져 있다고 간주합니다.
	void Redraw();

private:

	static CInterface* m_pInstance;

	//	해당 UI를 화면에 그립니다.
	void RenderUI(int index);

	//	UI 출력을 위한 멤버
	HANDLE m_hOP;
	
	string m_sBlank;
	string m_sUIs[WORLD_SIZE];
	string m_sName[WORLD_SIZE];
	int m_iUIColor[WORLD_SIZE];
	bool m_bAccupied[WORLD_SIZE] = { false, };
};
