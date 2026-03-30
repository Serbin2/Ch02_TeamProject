#pragma once
//	콘솔 창 위에 그래픽스를 표현하기 위한 클래스입니다

#include "../Standard.h"
#include <deque>

using namespace std;

enum Pixel
{
	blank = 0,
	square,
	triangle,
	circle,
	horizontalLine,
	verticalLine,
	cross,
	dust,
	star,
	Sniper,
	Skeleton,


	PixelMax,
};		//	미리 정의해야하는 픽셀 모양들입니다.

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

	static CGraphic* GetInstance();		//	싱글톤 인스턴스 가져오기
	static void Release();				//	사용이 끝나면 정리

	void Initialize();

	//	버퍼에 픽셀 하나를 그립니다.
	//	그리기 위치 X, Y, 그릴 문자 모양, 그리기 색
	void RenderToBuffer(int x, int y, int type, int color);

	//	버퍼를 화면에 출력합니다
	//	각 프레임의 시작에 StartDraw()를 호출합니다.
	//	각 프레임의 끝에 EndDraw()를 호출합니다.
	void StartDraw();
	void EndDraw();

	//	그리기 버퍼를 비워줍니다.
	//	화면을 다시 그려야 할 때 호출하여 이전 프레임 버퍼를 비우도록 해야합니다.
	void FlushingBuffer();

	//	로그를 추가합니다
	void AddLog(string str);

	//	화면을 모두 지우고 다시 그립니다.
	void ReDraw();

private:

	struct Shader
	{
		int vertex = Pixel::blank;
		int color = TEXT_BACKGROUND_BLACK;
	};

	//	초기화를 위한 함수들입니다.
	void SetPixelText();

	//	커서를 화면에 맞게 리사이징한 위치를 설정합니다.
	void SetCursorPos(int x, int y);

	//	버퍼에 배경색을 채웁니다
	void DrawBackground(int color);

	//	각 화면 영역의 테두리를 그립니다
	void DrawShape();

	//	로그 영역을 지웁니다
	void ClearLog();

	//	로그를 출력합니다.
	void PrintLog();

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
	string m_sBlank;				//	로그 문자열 정리용 공백문자열
	int m_iMaxLog;					//	출력할 수 있는 최대 로그 메세지 수 입니다.

	int m_DefaultBackgroundColor;	//	기본 배경 색입니다.

	bool m_bOnDraw;				//	그리기 중에만 작동해야 하는 함수들용
};