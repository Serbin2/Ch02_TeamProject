#include "MainMenu.h"
#include "../Standard.h"
#include <conio.h>
#include <cstdlib>
#include <windows.h>
#include <cmath>
#include "../Manager/SoundManager/SoundManager.h"

CMainMenu::CMainMenu()
	: cTitle(R"(██╗   ██╗███╗   ██╗████████╗██╗████████╗██╗     ███████╗██████╗
██║   ██║████╗  ██║╚══██╔══╝██║╚══██╔══╝██║     ██╔════╝██╔══██╗
██║   ██║██╔██╗ ██║   ██║   ██║   ██║   ██║     █████╗  ██║  ██║
██║   ██║██║╚██╗██║   ██║   ██║   ██║   ██║     ██╔══╝  ██║  ██║
╚██████╔╝██║ ╚████║   ██║   ██║   ██║   ███████╗███████╗██████╔╝
 ╚═════╝ ╚═╝  ╚═══╝   ╚═╝   ╚═╝   ╚═╝   ╚══════╝╚══════╝╚═════╝
██████╗ ██████╗  ██████╗
██╔══██╗██╔══██╗██╔════╝
██████╔╝██████╔╝██║  ███╗
██╔══██╗██╔═══╝ ██║   ██║
██║  ██║██║     ╚██████╔╝
╚═╝  ╚═╝╚═╝      ╚═════╝)"),
cCredits(R"( ██████╗ ██████╗ ███████╗██████╗ ██╗████████╗███████╗
██╔════╝ ██╔══██╗██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝
██║      ██████╔╝█████╗  ██║  ██║██║   ██║   ███████╗
██║      ██╔══██╗██╔══╝  ██║  ██║██║   ██║   ╚════██║
╚██████╗ ██║  ██║███████╗██████╔╝██║   ██║   ███████║
 ╚═════╝ ╚═╝  ╚═╝╚══════╝╚═════╝ ╚═╝   ╚═╝   ╚══════╝)"),
	cBaseArt(R"(                                                                       
                                                                                       
                            ..                                                         
                          :  --       :                                                
                          : :                =------==++**                             
                           : -    :#%%@@@@#=:..........:-=+*=                          
                              .   .#@.   :.........  .....::-+.                        
                                    #@  =:.  ...... ..  .....:--                       
                                     :%*-        .....     ...:=                       
                                        -:          .... ......:-                      
                                      .    .            .......:-                      
                                        .     .             ...:++                     
                                        :-:.     .            .:. @*    .              
                                         .-....      .        .=   %#.   ..            
                                           -:.....       .:::--   .%%%    -..          
                                             =-:.....:.       :*####*.     : :         
                                                :+==----+-    .            = .-        
                                                                         .   .         
                                                                                       
                                                                                       
                                                                                       )")
{
}

void CMainMenu::vDrawLitArt()
{
	const char* ramp = " .:-=+*#%@";
	const int rampCount = 10;

	int x = 0;
	int y = 0;

	const float centerX = 20.0f;
	const float centerY = 8.0f;

	float lightX = cosf(fLightAngle);
	float lightY = sinf(fLightAngle);
	float lightZ = 0.0f;

	for (char ch : cBaseArt)
	{
		if (ch == '\n')
		{
			std::cout << '\n';
			x = 0;
			++y;
			continue;
		}

		if (ch == ' ')
		{
			std::cout << ' ';
			++x;
			continue;
		}

		float dx = (float)x - centerX;
		float dy = (float)y - centerY;
		float dz = 0.0f;

		float len = sqrtf(dx * dx + dy * dy + 1.0f);
		float nx = dx / len;
		float ny = dy / len;
		float nz = dz / len;

		float light = nx * lightX + ny * lightY + nz * lightZ;
		light = (light + 1.0f) * 0.5f;

		int idx = (int)(light * (rampCount - 1));
		if (idx < 0) idx = 0;
		if (idx >= rampCount) idx = rampCount - 1;

		std::cout << ramp[idx];
		++x;
	}
}

void CMainMenu::vChoice()
{
	std::cout << cTitle << "\n\n";
	vDrawLitArt();
	std::cout << "\n\n";

	std::cout << "┌──────────────────┐\n";
	std::cout << (iCount == 0 ? "│    ▶ NEW GAME    │\n" : "│      NEW GAME    │\n");
	std::cout << (iCount == 1 ? "│    ▶ CREDITS     │\n" : "│      CREDITS     │\n");
	std::cout << (iCount == 2 ? "│    ▶ EXIT        │\n" : "│      EXIT        │\n");
	std::cout << "└──────────────────┘\n";
}

void CMainMenu::vCredit()
{
	std::cout << cCredits << "\n\n";
	std::cout << "        PRESS ENTER TO BACK\n";
}

void CMainMenu::vScene()
{
	if (!_kbhit())
		return;

	in = _getch();
	GET_SINGLE(CSoundManager)->PlaySFX(L"Select");

	if (scene == MENU)
	{
		if (in == 13)
		{
			if (iCount == 0) scene = START;
			else if (iCount == 1) scene = CREDIT;
			else if (iCount == 2) scene = EXIT;
		}
		else if (in == 0 || in == 224)
		{
			in = _getch();

			if (in == 72) iCount = (iCount + 2) % 3;
			else if (in == 80) iCount = (iCount + 1) % 3;
		}
	}
	else if (scene == CREDIT)
	{
		if (in == 13)
			scene = MENU;
	}
}

int CMainMenu::vRun()
{
	CONSOLE_CURSOR_INFO CurInfo;
	CurInfo.bVisible = FALSE;
	CurInfo.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);

	while (scene != START)
	{
		//system("cls");
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD(0, 0));
		if (scene == MENU)
		{
			fLightAngle += 0.08f;
			if (fLightAngle >= 6.283185f)
				fLightAngle -= 6.283185f;

			vChoice();
		}
		else if (scene == CREDIT)
		{
			vCredit();
		}
		else if (scene == EXIT)
		{
			return EXIT_GAME;
		}

		vScene();
		Sleep(60);
	}

	return GOTO_GAME;
}