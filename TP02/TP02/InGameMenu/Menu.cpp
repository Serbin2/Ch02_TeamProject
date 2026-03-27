#include "Menu.h"
#include <windows.h>
#include <conio.h>
#include <iostream>

int CMenu::ShowMenu()  // void -> int 로 변경
{
	SetConsoleTitle(L"게임 메뉴");
	while (true)
	{
		system("cls");

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(hConsole, &csbi);
		int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

		int menuWidth = 30;
		int menuCount = 8;

		std::string lines[] = {
			"==========================",
			"         게임 메뉴        ",
			"==========================",
			"       1.  게임  재개     ",
			"       2.  게임  종료     ",
			"       3.  상    점       ",
			"==========================",
			"  키보드 1, 2, 3 을 누르세요"
		};

		int startY = (height - menuCount) / 2;
		for (int i = 0; i < startY; i++)
			std::cout << "\n";

		int startX = (width - menuWidth) / 2;
		if (startX < 0) startX = 0;
		std::string pad(startX, ' ');

		for (int i = 0; i < menuCount; i++)
			std::cout << pad << lines[i] << "\n";

		while (GetAsyncKeyState('1') & 0x8000) Sleep(10);
		while (GetAsyncKeyState('2') & 0x8000) Sleep(10);
		while (GetAsyncKeyState('3') & 0x8000) Sleep(10);

		while (true)
		{
			if (GetAsyncKeyState('1') & 0x8000)
			{
				std::cout << "\n" << pad << "선택: 1 - 게임 재개\n";
				Sleep(500);
				while (GetAsyncKeyState('1') & 0x8000) Sleep(10);
				return 1;  // 게임 재개
			}
			if (GetAsyncKeyState('2') & 0x8000)
			{
				std::cout << "\n" << pad << "선택: 2 - 게임 종료\n";
				Sleep(500);
				while (GetAsyncKeyState('2') & 0x8000) Sleep(10);
				return 2;  // 게임 종료 → 메인메뉴로
			}
			if (GetAsyncKeyState('3') & 0x8000)
			{
				std::cout << "\n" << pad << "선택: 3 - 상점\n";
				Sleep(500);
				while (GetAsyncKeyState('3') & 0x8000) Sleep(10);
				return 3;  // 상점
			}
			Sleep(10);
		}
	}
}