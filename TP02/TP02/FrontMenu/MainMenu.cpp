// MainMenu.cpp
#include "MainMenu.h"
#include "../Standard.h"
#include <conio.h>
#include <cstdlib>

CMainMenu::CMainMenu() : cTitle(R"(██╗   ██╗███╗   ██╗████████╗██╗████████╗██╗     ███████╗██████╗  
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
╚═╝  ╚═╝╚═╝      ╚═════╝ )"), cCredits(R"( ██████╗ ██████╗ ███████╗██████╗ ██╗████████╗███████╗
██╔════╝ ██╔══██╗██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝
██║      ██████╔╝█████╗  ██║  ██║██║   ██║   ███████╗
██║      ██╔══██╗██╔══╝  ██║  ██║██║   ██║   ╚════██║
╚██████╗ ██║  ██║███████╗██████╔╝██║   ██║   ███████║
 ╚═════╝ ╚═╝  ╚═╝╚══════╝╚═════╝ ╚═╝   ╚═╝   ╚══════╝)") {}

void CMainMenu::vChoice() {
	std::cout << cTitle;
	std::cout << "\n┌──────────────────┐\n";
	std::cout << (iCount == 0 ? "│    ▶ NEW GAME    │\n" : "│      NEW GAME    │\n");
	std::cout << (iCount == 1 ? "│    ▶ CREDITS     │\n" : "│      CREDITS     │\n");
	std::cout << (iCount == 2 ? "│    ▶ EXIT        │\n" : "│      EXIT        │\n");
	std::cout << "└──────────────────┘\n";
}
void CMainMenu::vCredit() {
	std::cout << cCredits;
}
void CMainMenu::vScene() {
	in = _getch();

	if (scene == MENU) { // start 일때 화살표 키로 iCount 
		if (in == 13) {
			if (iCount == 0) scene = START;
			else if (iCount == 1) scene = CREDIT;
			else if (iCount == 2) scene = EXIT;
		}

		if (in == 0 || in == 224) {
			in = _getch();

			if (in == 72) iCount = (iCount + 2) % 3;
			else if (in == 80) iCount = (iCount + 1) % 3;
		}
	}
	else if (scene == CREDIT) {
		if (in == 13) scene = MENU; // enter 일떄 back to start 
	}

}
int CMainMenu::vRun() 
{
	while (scene != START) {
		system("cls");

		if (scene == MENU) vChoice();
		else if (scene == CREDIT) vCredit();
		else if (scene == EXIT) return EXIT_GAME;

		vScene();
	}

	return GOTO_GAME;
}