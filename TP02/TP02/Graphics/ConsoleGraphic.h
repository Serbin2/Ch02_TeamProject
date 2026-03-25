#pragma once
//	콘솔 창 위에 그래픽스를 표현하기 위한 클래스입니다

#include "../Standard.h"

using namespace std;

class CGraphic
{
private:
	CGraphic();
	~CGraphic();

	typedef enum
	{
		blank = 0,
		square,
		triangle,
		circle,
		horizontalLine,
		verticalLine,



		PixelMax,
	}Pixel;

	typedef enum
	{
		TEXT_FOREGROUND_BLUE		=	FOREGROUND_BLUE,
		TEXT_FOREGROUND_GREEN		=	FOREGROUND_GREEN,
		TEXT_FOREGROUND_RED			=	FOREGROUND_RED,
		TEXT_FOREGROUND_YELLOW		=	FOREGROUND_RED|FOREGROUND_GREEN,
		TEXT_FOREGROUND_MAGENTA		=	FOREGROUND_BLUE|FOREGROUND_RED,
		TEXT_FOREGROUND_CYAN		=	FOREGROUND_BLUE|FOREGROUND_GREEN,
		TEXT_FOREGROUND_GRAY		=	TEXT_FOREGROUND_RED|TEXT_FOREGROUND_BLUE|TEXT_FOREGROUND_GREEN,
		TEXT_FOREGROUND_WHITE		=	TEXT_FOREGROUND_GRAY|FOREGROUND_INTENSITY,
		TEXT_FOREGROUND_BLACK		=	0,
											
		TEXT_BACKGROUND_BLUE		=	BACKGROUND_BLUE,
		TEXT_BACKGROUND_GREEN		=	BACKGROUND_GREEN,
		TEXT_BACKGROUND_RED			=	BACKGROUND_RED,
		TEXT_BACKGROUND_YELLOW		=	BACKGROUND_RED|BACKGROUND_GREEN,
		TEXT_BACKGROUND_MAGENTA		=	BACKGROUND_BLUE|BACKGROUND_RED,
		TEXT_BACKGROUND_CYAN		=	BACKGROUND_BLUE|BACKGROUND_GREEN,
		TEXT_BACKGROUND_GRAY		=	TEXT_BACKGROUND_RED|TEXT_BACKGROUND_BLUE|TEXT_BACKGROUND_GREEN,
		TEXT_BACKGROUND_WHITE		=	TEXT_BACKGROUND_GRAY|BACKGROUND_INTENSITY,
		TEXT_BACKGROUND_BLACK		=	0,
	}TextColor;

	//	픽셀 출력용 스트링
	string m_sPixels[Pixel::PixelMax];

	void SetPixelText();
public:

	static CGraphic* GetInstance();
	static void Release();
	void SetCursorPos(int x, int y);
	void SetScreenSize(int x, int y);
	void PrintText(string str);
	string GetPixelType(Pixel type);

	void Render();

protected:
	
	static CGraphic* m_pInstance;

	HANDLE m_hOP;						//	console output handle
	//CONSOLE_CURSOR_INFO	m_CurInfo;		//	cursor info

	vector<vector<string>> m_aBuffer;	//	buff
};


// 한글 주석 테스트 한글 주석 테스트 / 특수문자 테스트 !@#$ #$%^&%^& / 영어 주석 테스트 asdfasdfasdfasdf