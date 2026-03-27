#pragma once
#include <iostream>
#include <conio.h>
#include <cstdlib>

class cMainMenu {
private:
	enum eScene { START, CREDIT, EXIT };
	eScene scene = START;
	int iCount = 0;
	int in = 0;
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
		std::cout << (iCount == 0 ? "│    ▶ NEW GAME    │\n" : "│      NEW GAME    │\n");
		std::cout << (iCount == 1 ? "│    ▶ CREDITS     │\n" : "│      CREDITS     │\n");
		std::cout << (iCount == 2 ? "│    ▶ EXIT        │\n" : "│      EXIT        │\n");
		std::cout << "└──────────────────┘\n";
	}

	void vCredit() {
		std::cout << cCredits;
	}

	void vScene() {
		in = _getch();

		if (scene == START) {
			if (in == 13) {
				if (iCount == 1) scene = CREDIT;
				else if (iCount == 2) scene = EXIT;
			}

			if (in == 0 || in == 224) {
				in = _getch();

				if (in == 72) iCount = (iCount + 2) % 3;
				else if (in == 80) iCount = (iCount + 1) % 3;
			}
		}
		else if (scene == CREDIT) {
			if (in == 13) scene = START;
		}
	}

public:
	void vRun() {
		while (scene != EXIT) {
			system("cls");

			if (scene == START) vChoice();
			else if (scene == CREDIT) vCredit();

			vScene();
		}
	}
};