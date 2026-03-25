#include "ConsoleGraphic.h"

CGraphic* CGraphic::m_pInstance = nullptr;

CGraphic::CGraphic()
{
	//	콘솔 출력에 대한 기본 핸들을 가져옵니다
	m_hOP = GetStdHandle(STD_OUTPUT_HANDLE);
	SetPixelText();
	m_iScreenSize = 30;

	//	커서 투명화
	m_CurInfo.bVisible = 0;
	m_CurInfo.dwSize = 1;
	SetConsoleCursorInfo(m_hOP, &m_CurInfo);
	
	//	버퍼를 준비합니다.
	for (int i = 0; i < 30; i++)
	{
		buffer[i].resize(61, ' ');
	}

	m_iMaxLog = 5;
	DrawShape();
}

CGraphic::~CGraphic()
{

}

void CGraphic::SetPixelText()
{//	여기서 픽셀의 형태를 정의하세요
	m_sPixels[blank] = "  ";
	m_sPixels[square] = "HH";
	m_sPixels[triangle] = "<>";
	m_sPixels[circle] = "()";
	m_sPixels[horizontalLine] = "--";
	m_sPixels[verticalLine] = "||";
	m_sPixels[cross] = "++";
}

CGraphic* CGraphic::GetInstance()
{//	싱글톤 구조입니다
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CGraphic();
	}

	return m_pInstance;
}

void CGraphic::Release()
{//	사용이 종료되면 삭제해주세요
	if (m_pInstance == nullptr)
	{
		return;
	}

	delete m_pInstance;
	m_pInstance = nullptr;
}

//	커서의 위치를 변경합니다
void CGraphic::SetCursorPos(int x, int y)
{	
	COORD cdPos;
	cdPos.X = x * 2;
	cdPos.Y = y;
	SetConsoleCursorPosition(m_hOP, cdPos);
}

void CGraphic::RenderToBuffer(int x, int y, CGraphic::Pixel type, CGraphic::TextColor color)
{

}

//	버퍼를 화면에 그립니다.
void CGraphic::Draw()
{
	for (int i = 0; i < 30; i++)
	{
		SetCursorPos(1, i + 1);
		cout << buffer[i];
	}
}

void CGraphic::AddLog(string str)
{
	ClearLog();
	m_aLog.push_back(str);
	while(m_aLog.size() > m_iMaxLog)
	{
		m_aLog.pop_front();
	}

	int logSize = (int)m_aLog.size();
	for (int i = 0; i < logSize; i++)
	{
		SetCursorPos( 1, m_iScreenSize + 4 + i);
		cout << m_aLog[i];
	}
}

void CGraphic::SetMaxLog(int max)
{
	m_iMaxLog = max;
	DrawLog();
}

void CGraphic::DrawShape()
{
	DrawMainScreen();
	DrawUI();
	DrawLog();
}

void CGraphic::DrawMainScreen()
{
	SetConsoleTextAttribute(m_hOP, TextColor::TEXT_BACKGROUND_WHITE);
	// 창 테두리 시작 위치
	point start;
	// 창 테두리 끝 위치
	point end;
	start.Y = 0;

	end.Y = m_iScreenSize + 2;
	start.X = 0;
	end.X = m_iScreenSize + 2;

	for (int x = start.X; x < end.X; x++)
	{
		SetCursorPos(x, start.Y);
		cout << m_sPixels[Pixel::blank];
		SetCursorPos(x, end.Y);
		cout << m_sPixels[Pixel::blank];
	}
	for (int y = start.Y; y <= end.Y; y++)
	{
		SetCursorPos(start.X, y);
		cout << m_sPixels[Pixel::blank];
		SetCursorPos(end.X, y);
		cout << m_sPixels[Pixel::blank];
	}
	SetConsoleTextAttribute(m_hOP, TextColor::TEXT_BACKGROUND_BLACK | TextColor::TEXT_FOREGROUND_WHITE);
}

void CGraphic::DrawUI()
{
	SetConsoleTextAttribute(m_hOP, TextColor::TEXT_BACKGROUND_WHITE);
	//	ui 창 테두리 시작 위치
	point start;
	//	ui 창 테두리 끝 위치
	point end;
	start.Y = 0;

	end.Y = m_iScreenSize + 2;
	start.X = m_iScreenSize + 3;
	end.X = m_iScreenSize + 2 + 10 + 2;

	for (int x = start.X; x < end.X; x++)
	{
		SetCursorPos(x, start.Y);
		cout << m_sPixels[Pixel::blank];
		SetCursorPos(x, end.Y);
		cout << m_sPixels[Pixel::blank];
	}
	for (int y = start.Y; y <= end.Y; y++)
	{
		SetCursorPos(start.X, y);
		cout << m_sPixels[Pixel::blank];
		SetCursorPos(end.X, y);
		cout << m_sPixels[Pixel::blank];
	}
	SetConsoleTextAttribute(m_hOP, TextColor::TEXT_BACKGROUND_BLACK | TextColor::TEXT_FOREGROUND_WHITE);
}

void CGraphic::ClearLog()
{
	string str;
	str.resize(100, ' ');

	SetConsoleTextAttribute(m_hOP, TextColor::TEXT_BACKGROUND_BLACK);
	for (int i = m_iScreenSize + 4; i <= m_iScreenSize + m_iMaxLog + 3; i++)
	{
		SetCursorPos(0, i);
		cout << str;
	}
	SetConsoleTextAttribute(m_hOP, TextColor::TEXT_BACKGROUND_BLACK | TextColor::TEXT_FOREGROUND_WHITE);
}

void CGraphic::DrawLog()
{
	ClearLog();
	//	로그 창 테두리 시작 위치
	point start;
	//	로그 창 테두리 끝 위치
	point end;
	start.Y = m_iScreenSize + 3;
	end.Y = start.Y + m_iMaxLog + 1;
	start.X = 0;
	end.X = m_iScreenSize + 2 + 10 + 2;

	SetConsoleTextAttribute(m_hOP, TextColor::TEXT_BACKGROUND_WHITE);
	for (int x = start.X; x <= end.X; x++)
	{
		SetCursorPos(x, start.Y);
		cout << m_sPixels[Pixel::blank];
		SetCursorPos(x, end.Y);
		cout << m_sPixels[Pixel::blank];
	}
	SetConsoleTextAttribute(m_hOP, TextColor::TEXT_BACKGROUND_BLACK | TextColor::TEXT_FOREGROUND_WHITE);
}