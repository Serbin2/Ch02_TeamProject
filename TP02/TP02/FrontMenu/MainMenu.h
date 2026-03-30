#pragma once
#include <iostream>

class CMainMenu {
private:
	enum eScene { MENU, START, CREDIT, EXIT };
	eScene scene = MENU;
	int iCount = 0;
	int in = 0;

	const char* cTitle;
	const char* cCredits;

	void vChoice();
	void vCredit();
	void vScene();

public:
	CMainMenu();
	void vRun();
};