#pragma once
#include <iostream>
#include <string>

class CMainMenu
{
private:
	enum eScene { MENU, START, CREDIT, EXIT };
	eScene scene = MENU;

	int iCount = 0;
	int in = 0;
	float fLightAngle = 0.0f;

	const char* cTitle;
	const char* cCredits;
	std::string cBaseArt;

	void vChoice();
	void vCredit();
	void vScene();
	void vDrawLitArt();

public:
	CMainMenu();
	int vRun();
};