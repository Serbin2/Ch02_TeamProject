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
	}Pixel;		//	미리 정의해야하는 픽셀 모양들입니다.

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

	}TextColor;	//	텍스트 컬러입니다. 글자색|배경색 -> 비트연산자로 합성하세요

	static CGraphic* GetInstance();		//	싱글톤 인스턴스 가져오기
	static void Release();				//	사용이 끝나면 정리


	//	버퍼에 픽셀 하나를 그립니다.
	void RenderToBuffer(int x, int y, Pixel type, TextColor color);

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

	//	초기화를 위한 함수들입니다.
	void SetPixelText();

	//	커서를 화면에 맞게 리사이징한 위치를 설정합니다.
	void SetCursorPos(int x, int y);

	//	버퍼에 배경색을 채웁니다
	void DrawBackground(TextColor color);

	//	각 화면 영역의 테두리를 그립니다
	void DrawShape();

	//	로그 영역을 지웁니다
	void ClearLog();

protected:

	static CGraphic* m_pInstance;		//	싱글톤 인스턴스

	HANDLE m_hOP;						//	콘솔 아웃풋 핸들
	CONSOLE_CURSOR_INFO	m_CurInfo;		//	커서 정보

	string m_sPixels[Pixel::PixelMax];	//	픽셀 출력용 스트링

	vector<vector<Shader>> m_aPrevBuffer;	//	더블 버퍼링을 위한 이전버퍼
	vector<vector<Shader>> m_aBuffer;		//	메인 버퍼
	int m_iScreenSize;						//	메인 화면의 크기(정사각형)
	int m_iEndOfScreenX;					//	게임창의 끝 위치
	int m_iEndOfScreenY;					//	게임창의 끝 위치

	deque<string> m_aLog;			//	로그 큐
	int m_iMaxLog;					//	출력할 수 있는 최대 로그 메세지 수 입니다.

	TextColor m_DefaultBackgroundColor;	//	기본 배경 색입니다.

	bool m_bOnDraw;				//	그리기 중에만 작동해야 하는 함수들용
};