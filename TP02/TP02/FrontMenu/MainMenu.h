#pragma once
#include <iostream>
#include <conio.h>
#include <cstdlib>

class cMainMenu {
private:
	enum eScene { MENU ,START, CREDIT, EXIT };
	eScene scene = MENU;
	int m_iMaxAmount = 0;
	int in = 0;

	//타이틀에 넣고싶은거 넣기
	const char* cTitle = R"(██╗   ██╗███╗   ██╗████████╗██╗████████╗██╗     ███████╗██████╗  
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
╚═╝  ╚═╝╚═╝      ╚═════╝ )";
	//크래딧에 넣고싶은거 넣기
	const char* cCredits = R"( ██████╗ ██████╗ ███████╗██████╗ ██╗████████╗███████╗
██╔════╝ ██╔══██╗██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝
██║      ██████╔╝█████╗  ██║  ██║██║   ██║   ███████╗
██║      ██╔══██╗██╔══╝  ██║  ██║██║   ██║   ╚════██║
╚██████╗ ██║  ██║███████╗██████╔╝██║   ██║   ███████║
 ╚═════╝ ╚═╝  ╚═╝╚══════╝╚═════╝ ╚═╝   ╚═╝   ╚══════╝
)";

	void vChoice() {
		std::cout << cTitle;
		std::cout << "\n┌──────────────────┐\n";
		std::cout << (m_iMaxAmount == 0 ? "│    ▶ NEW GAME    │\n" : "│      NEW GAME    │\n");
		std::cout << (m_iMaxAmount == 1 ? "│    ▶ CREDITS     │\n" : "│      CREDITS     │\n");
		std::cout << (m_iMaxAmount == 2 ? "│    ▶ EXIT        │\n" : "│      EXIT        │\n");
		std::cout << "└──────────────────┘\n";
	}

	void vCredit() {
		std::cout << cCredits;
	}

	void vScene() {
		in = _getch();

		if (scene == MENU) { // start 일때 화살표 키로 iCount 
			if (in == 13) {
				if (m_iMaxAmount == 0) scene = START;
				else if (m_iMaxAmount == 1) scene = CREDIT;
				else if (m_iMaxAmount == 2) scene = EXIT;
			}

			if (in == 0 || in == 224) {
				in = _getch();

				if (in == 72) m_iMaxAmount = (m_iMaxAmount + 2) % 3;
				else if (in == 80) m_iMaxAmount = (m_iMaxAmount + 1) % 3;
			}
		}
		else if (scene == CREDIT) {
			if (in == 13) scene = MENU; // enter 일떄 back to start 
		}
	}

public:
	void vRun() {
		while (scene != START) {
			system("cls");

			if (scene == MENU) vChoice();
			else if (scene == CREDIT) vCredit();
			else if (scene == EXIT) exit(0);

			vScene();
		}
	}
};