//	Window 11 이상의 콘솔창은 콘솔 창 크기 강제 조절이 안됩니다.
//	콘솔 창의 설정에서 기본 시작 크기를 조정해서 게임플레이의 적당한 크기를 미리 설정하고 다시 실행해주세요.
//	260325 지호나
#include "Graphics/ConsoleGraphic.h"
#include "Input/Input.h"
#include "Time/Timer.h"
#include "Menu.h"
#include <windows.h>
#include <iostream>
#include <conio.h>

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	std::cout << "윈도우 10 이상의 콘솔창은 코드에서의 콘솔 창 크기 강제 조절이 동작하지 않을 수 있습니다.\n";
	std::cout << "게임을 시작하기 전에 콘솔 창 크기를 적절하게 조절 한 후 진행하시기 바랍니다.\n";
	std::cout << "아무 키를 눌러 계속합니다.\n";
	char in = _getch();

	CGraphic* pGraphic = CGraphic::GetInstance();
	CInput* pInput = CInput::GetInstance();
	CTimer Timer;

	if (!pGraphic || !pInput) return 0;

	int pix = Pixel::square;
	int tex = TEXT_BACKGROUND_MAGENTA | TEXT_FOREGROUND_CYAN;
	int x = 15;
	int y = 15;

	Timer.Start();

	while (1)
	{
		double deltaTime = Timer.Update();
		pInput->Update();

		if (pInput->IsKeyDown('W')) y--;
		if (pInput->IsKeyDown('A')) x--;
		if (pInput->IsKeyDown('S')) y++;
		if (pInput->IsKeyDown('D')) x++;

		if (pInput->IsKeyDown('G'))
		{
			pGraphic->AddLog("G키를 눌렀습니다.");
			CMenu menu;
			menu.ShowMenu();
			Timer.Start();
		}

		pGraphic->StartDraw();
		pGraphic->RenderToBuffer(x, y, pix, tex);
		pGraphic->EndDraw();

		if (pInput->IsKeyDown(VK_ESCAPE)) break;
	}

	CGraphic::Release();
	CInput::Release();
	return 0;
}
