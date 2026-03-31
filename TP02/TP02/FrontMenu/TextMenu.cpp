#include "TextMenu.h"
#include "../Standard.h"
#include "../Manager/SoundManager/SoundManager.h"

using namespace std;

static int VisWidth(const std::string& s)
{
	int width = 0;
	int i = 0;
	while (i < (int)s.size())
	{
		unsigned char c = (unsigned char)s[i];
		if ((c & 0xF0) == 0xE0)
		{
			// 한글 (UTF-8 3바이트) → 콘솔 2칸
			width += 2;
			i += 3;
		}
		else
		{
			// ASCII, 숫자, 특수문자 → 콘솔 1칸
			width += 1;
			i += 1;
		}
	}
	return width;
}

void CTextMenu::Intro()
{
	HANDLE hOP = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOP, COORD(0, 0));
	system("cls");
	
	string str1 = "연료 고갈, 엔진 정지. 불시착한 곳은 기계들의 무덤\n";
	string str2 = "미쳐버린 로봇들이 몰려온다. 살고 싶다면 놈들을 부수고 고철을 뺏어라\n";
	string str3 = "이 행성을 탈출할 연료는... 오직 놈들의 잔해뿐이다\n";
	string text = str1 + str2 + str3;
	int textWidth = VisWidth(text);
	int innerWidth = textWidth + 2;

	// 타이핑 효과: 한글은 3바이트를 한 단위로 출력해야 글씨 안깨짐
	int i = 0;
	while (i < (int)text.size())
	{
		unsigned char c = (unsigned char)text[i];
		if ((c & 0xF0) == 0xE0) // 한글 시작 바이트 판별
		{
			std::cout << text[i] << text[i + 1] << text[i + 2] << std::flush;
			i += 3;
		}
		else
		{
			std::cout << text[i] << std::flush;
			i += 1;
		}
		GET_SINGLE(CSoundManager)->PlaySFX(L"String");
		Sleep(80);
	}
	//	TODO 여기에 인트로 사운트 추가
	GET_SINGLE(CSoundManager)->PlaySFX(L"Fall");
	while (_kbhit())
	{
		_getch();
	}
	_getch();
}

void CTextMenu::Outro()
{
	HANDLE hOP = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOP, COORD(0, 0));
	system("cls");

	string str1 = "그는 결국 탈출했다.\n";
	string str2 = "그의 뒤에 남은건 부서진 철조각들 뿐이었다.\n";
	string str3 = "                          end\n";
	string text = str1 + str2 + str3;
	int textWidth = VisWidth(text);
	int innerWidth = textWidth + 2;

	// 타이핑 효과: 한글은 3바이트를 한 단위로 출력해야 글씨 안깨짐
	int i = 0;
	while (i < (int)text.size())
	{
		unsigned char c = (unsigned char)text[i];
		if ((c & 0xF0) == 0xE0) // 한글 시작 바이트 판별
		{
			std::cout << text[i] << text[i + 1] << text[i + 2] << std::flush;
			i += 3;
		}
		else
		{
			std::cout << text[i] << std::flush;
			i += 1;
		}
		GET_SINGLE(CSoundManager)->PlaySFX(L"String");
		Sleep(80);
	}
	//	TODO 여기에 아웃트로 사운트 추가
	GET_SINGLE(CSoundManager)->PlaySFX(L"Leave");
	while (_kbhit())
	{
		_getch();
	}
	_getch();
}