#include "TextMenu.h"
#include "../Standard.h"
#include "../Manager/SoundManager/SoundManager.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <vector>

using namespace std;


//  콘솔 색상 Windows API SetConsoleTextAttribute 사용

#define CRT_DIM    (FOREGROUND_GREEN)
#define CRT_BRIGHT (FOREGROUND_GREEN | FOREGROUND_INTENSITY)

static void SetColor(WORD color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// 모든 출력이 끝난 뒤 반드시 호출해서 다음 출력이 깨지지 않게 복원

static void ResetColor()
{
	SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}


//  창 크기가 바뀌어도 자동으로 맞춰짐

// contentWidth : 출력할 내용의 실제 글자 너비(칸 수)
// 반환값       : 왼쪽에 붙여야 할 공백 문자열

static string GetPad(int contentWidth)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;

	int left = (consoleWidth - contentWidth) / 2;
	if (left < 0) left = 0;

	return string(left, ' ');
}

// lineCount : 출력할 전체 줄 수
// 콘솔 세로 중앙에 맞추기 위해 위쪽에 빈 줄을 미리 출력

static void PrintTopPadding(int lineCount)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	int top = (consoleHeight - lineCount) / 2;
	if (top < 0) top = 0;

	for (int i = 0; i < top; i++)
		cout << "\n";
}


//  한글 한 글자는 영문 2칸을 차지

static int VisWidth(const string& s)
{
	int width = 0;
	int i = 0;
	while (i < (int)s.size())
	{
		unsigned char c = (unsigned char)s[i];
		if ((c & 0xF0) == 0xE0)
		{
			// 한글  → 콘솔 2칸
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


//  타이핑 효과 출력

static void TypeLine(const string& pad, const string& line)
{
	cout << pad;
	int i = 0;
	while (i < (int)line.size())
	{
		unsigned char c = (unsigned char)line[i];
		if ((c & 0xF0) == 0xE0) // 한글 시작 바이트 판별
		{
			cout << line[i] << line[i + 1] << line[i + 2] << flush;
			i += 3;
		}
		else
		{
			cout << line[i] << flush;
			i += 1;
		}
		GET_SINGLE(CSoundManager)->PlaySFX(L"String");
		Sleep(60);
	}
	cout << "\n";
}




// "> ----...---- <" 형태의 가로선

static void PrintDivider(const string& pad, int boxW)
{
	// "> " 2칸 + 대시 + " <" 2칸 = boxW 칸
	int dashCount = boxW - 4;
	if (dashCount < 0) dashCount = 0;

	SetColor(CRT_DIM); // 구분선은 어두운 녹색으로 배경처럼 표현
	cout << pad << "> " << string(dashCount, '-') << " <\n";
	ResetColor();
}


//  ShowCRTScreen — Intro/Outro 공용 화면 출력
//
//  tag    : 화면 상단 태그 
//  lines  : 본문 줄 목록
//  prompt : 화면 하단
//
//
//    > BOOT SEQUENCE COMPLETE_        ← 태그 (밝은 녹색)
//    > -------------------------------- <  ← 구분선 (어두운 녹색)
//    >  연료 고갈, 엔진 정지.         ← 본문 타이핑 (밝은 녹색)
//    >  불시착한 곳은 기계들의 무덤
//    > -------------------------------- <  ← 구분선 (어두운 녹색)
//    > PRESS ANY KEY TO CONTINUE_     ← 프롬프트 (어두운 녹색)

static void ShowCRTScreen(
	const string& tag,
	const vector<string>& lines,
	const string& prompt)
{
	system("cls");

	// 박스 너비 = 가장 긴 줄 기준, 최소 50칸 보장
	int boxW = 50;
	boxW = max(boxW, VisWidth(tag));
	boxW = max(boxW, VisWidth(prompt));
	for (auto& l : lines)
		boxW = max(boxW, VisWidth(l) + 3); // ">  " 3칸 포함

	string pad = GetPad(boxW);

	// 전체 줄 수 = 태그(1) + 위 구분선(1) + 본문 줄 수 + 아래 구분선(1) + 프롬프트(1)
	int totalLines = 4 + (int)lines.size();
	PrintTopPadding(totalLines);

	// ── 태그 출력 (밝은 녹색)
	SetColor(CRT_BRIGHT);
	cout << pad << tag << "\n";

	// ── 위 구분선
	PrintDivider(pad, boxW);

	// ── 본문 타이핑 출력 (밝은 녹색, 한 줄씩)
	SetColor(CRT_BRIGHT);
	for (auto& line : lines)
		TypeLine(pad, ">  " + line); // ">  " 접두사로 터미널 느낌

	// ── 아래 구분선
	PrintDivider(pad, boxW);

	// ── 프롬프트 출력 (어두운 녹색으로 구분)
	SetColor(CRT_DIM);
	cout << pad << prompt << "\n";

	ResetColor();
}



//  게임 시작 전 스토리 출력
//  대사를 추가/수정하려면 lines 배열만 편집

void CTextMenu::Intro()
{
	vector<string> lines =
	{
		"연료 고갈, 엔진 정지.",
		"불시착한 곳은 기계들의 무덤",
		"미쳐버린 로봇들이 몰려온다. 살고 싶다면 놈들을 부수고 고철을 뺏어라",
		"이 행성을 탈출할 연료는... 오직 놈들의 잔해뿐이다",
	};

	ShowCRTScreen("> SYSTEM ERROR : EMERGENCY LANDING_", lines, "> PRESS ANY KEY TO CONTINUE_");

	//	TODO 여기에 인트로 사운드 추가
	GET_SINGLE(CSoundManager)->PlaySFX(L"Fall");
	while (_kbhit()) _getch();
	_getch();
}



//  게임 클리어 후 엔딩 메시지 출력
//  대사를 추가/수정하려면 lines 배열만 편집

void CTextMenu::Outro()
{
	vector<string> lines =
	{
		"그는 결국 탈출했다.",
		"그의 뒤에 남은건 부서진 철조각들 뿐이었다.",
		"                          end",
	};

	ShowCRTScreen("> MISSION COMPLETE_", lines, "> PRESS ANY KEY TO EXIT_");

	//	TODO 여기에 아웃트로 사운드 추가
	GET_SINGLE(CSoundManager)->PlaySFX(L"Leave");
	while (_kbhit()) _getch();
	_getch();
}