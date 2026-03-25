#pragma once
//	콘솔 창 위에 그래픽스를 표현하기 위한 클래스입니다

#include "../Standard.h"
#include <deque>

using namespace std;

class CGraphic
{
private:
	CGraphic();
	~CGraphic();

	struct point
	{
		int X;
		int Y;
	};

public:

	typedef enum
	{
		blank = 0,
		square,
		triangle,
		circle,
		horizontalLine,
		verticalLine,
		cross,


		PixelMax,
	}Pixel;

	typedef enum
	{
		TEXT_FOREGROUND_BLUE = FOREGROUND_BLUE,
		TEXT_FOREGROUND_GREEN = FOREGROUND_GREEN,
		TEXT_FOREGROUND_RED = FOREGROUND_RED,
		TEXT_FOREGROUND_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
		TEXT_FOREGROUND_MAGENTA = FOREGROUND_BLUE | FOREGROUND_RED,
		TEXT_FOREGROUND_CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN,
		TEXT_FOREGROUND_GRAY = TEXT_FOREGROUND_RED | TEXT_FOREGROUND_BLUE | TEXT_FOREGROUND_GREEN,
		TEXT_FOREGROUND_WHITE = TEXT_FOREGROUND_GRAY | FOREGROUND_INTENSITY,
		TEXT_FOREGROUND_BLACK = 0,

		TEXT_BACKGROUND_BLUE = BACKGROUND_BLUE,
		TEXT_BACKGROUND_GREEN = BACKGROUND_GREEN,
		TEXT_BACKGROUND_RED = BACKGROUND_RED,
		TEXT_BACKGROUND_YELLOW = BACKGROUND_RED | BACKGROUND_GREEN,
		TEXT_BACKGROUND_MAGENTA = BACKGROUND_BLUE | BACKGROUND_RED,
		TEXT_BACKGROUND_CYAN = BACKGROUND_BLUE | BACKGROUND_GREEN,
		TEXT_BACKGROUND_GRAY = TEXT_BACKGROUND_RED | TEXT_BACKGROUND_BLUE | TEXT_BACKGROUND_GREEN,
		TEXT_BACKGROUND_WHITE = TEXT_BACKGROUND_GRAY | BACKGROUND_INTENSITY,
		TEXT_BACKGROUND_BLACK = 0,

	}TextColor;

	static CGraphic* GetInstance();
	static void Release();

	string GetPixelTypeToString(Pixel type) { return m_sPixels[type]; };

	void RenderToBuffer(int x, int y, Pixel type, TextColor color);
	void DrawBackground(TextColor color);

	//	버퍼를 화면에 출력합니다
	//	각 프레임의 시작에 StartDraw()를 호출합니다.
	//	각 프레임의 끝에 EndDraw()를 호출합니다.
	void StartDraw();
	void EndDraw();

	//	로그를 추가합니다
	void AddLog(string str);
	

private:

	struct Shader
	{
		Pixel vertex = blank;
		TextColor color = TEXT_BACKGROUND_BLACK;
	};

	void SetPixelText();
	void SetCursorPos(int x, int y);

	//	각 화면 영역의 테두리를 그립니다
	void DrawShape();
	void ClearLog();

protected:

	//	픽셀 출력용 스트링
	string m_sPixels[Pixel::PixelMax];
	
	static CGraphic* m_pInstance;

	HANDLE m_hOP;						//	console output handle
	CONSOLE_CURSOR_INFO	m_CurInfo;		//	cursor info

	vector<vector<Shader>> m_aPrevBuffer;	//	buff
	vector<vector<Shader>> m_aBuffer;	//	double Buffering
	int m_iScreenSize;
	int m_iEndOfScreenX;
	int m_iEndOfScreenY;

	deque<string> m_aLog;
	int m_iMaxLog;		//	출력할 수 있는 최대 로그 메세지 수 입니다.

	TextColor m_DefaultBackgroundColor;	//	기본 배경 색입니다.
};


// 한글 주석 테스트 한글 주석 테스트 / 특수문자 테스트 !@#$ #$%^&%^& / 영어 주석 테스트 asdfasdfasdfasdf