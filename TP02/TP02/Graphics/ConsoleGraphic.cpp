#include "ConsoleGraphic.h"

CGraphic* CGraphic::m_pInstance = nullptr;

CGraphic::CGraphic()
{
	//	콘솔 출력에 대한 기본 핸들을 가져옵니다
	m_hOP = GetStdHandle(STD_OUTPUT_HANDLE);
	SetPixelText();
	m_iScreenSize = 30;
	m_iEndOfScreenX = 43;
	m_iEndOfScreenY = 40;

	//	커서 투명화
	m_CurInfo.bVisible = 0;
	m_CurInfo.dwSize = 1;
	SetConsoleCursorInfo(m_hOP, &m_CurInfo);

	//	버퍼를 준비합니다
	m_aPrevBuffer.resize(30);
	m_aBuffer.resize(30);
	for (int i = 0; i < 30; i++)
	{
		m_aPrevBuffer[i].resize(30);
		m_aBuffer[i].resize(30);
	}

	//	멤버 변수들을 초기화합니다
	m_iMaxLog = 7;
	m_DefaultBackgroundColor = TextColor::TEXT_BACKGROUND_BLUE;
	m_bOnDraw = false;

	//	게임 레이아웃을 그려둡니다
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
	if (!m_bOnDraw)	return;
	m_aBuffer[y][x].color = color;
	m_aBuffer[y][x].vertex = type;
}

//	배경을 그립니다.
void CGraphic::DrawBackground(CGraphic::TextColor color)
{
	if (!m_bOnDraw)	return;
	Shader s;
	s.color = color;
	for (auto& i : m_aBuffer)
	{
		fill(i.begin(), i.end(), s);
	}
}

void CGraphic::StartDraw()
{
	if (m_bOnDraw)	return;	//	StartDraw를 연속해서 두번 호출하지 마시오
	m_bOnDraw = true;
	DrawBackground(m_DefaultBackgroundColor);
}

//	버퍼를 화면에 그립니다.
void CGraphic::EndDraw()
{
	if (!m_bOnDraw)	return;	//	그리기중이 아닙니다.

	for (int y = 0; y < 30; y++)
	{
		for (int x = 0; x < 30; x++)
		{
			//	이전 화면과 같은 내용이면 넘어갑니다
			if (m_aPrevBuffer[y][x].vertex == m_aBuffer[y][x].vertex &&
				m_aPrevBuffer[y][x].color == m_aBuffer[y][x].color)	continue;

			//	픽셀을 그립니다
			SetCursorPos(x + 1, y + 1);
			SetConsoleTextAttribute(m_hOP, m_aBuffer[y][x].color);
			cout << m_sPixels[m_aBuffer[y][x].vertex];
		}
	}

	//	그리기가 끝나면 색상을 초기화 합니다
	SetConsoleTextAttribute(m_hOP, TEXT_FOREGROUND_WHITE | TEXT_BACKGROUND_BLACK);
	//	더블버퍼링
	swap(m_aPrevBuffer, m_aBuffer);

	m_bOnDraw = false;	//	그리기가 끝났습니다.
}

//	로그를 출력합니다
void CGraphic::AddLog(string str)
{
	//	로그창을 지웁니다.
	ClearLog();

	//	새 로그를 큐에 넣습니다
	m_aLog.push_back(str);
	while (m_aLog.size() > m_iMaxLog)
	{	//	큐가 넘치면 오래된 로그를 제거합니다.
		m_aLog.pop_front();
	}

	//	로그를 다시 출력합니다.
	int logSize = (int)m_aLog.size();
	for (int i = 0; i < logSize; i++)
	{
		SetCursorPos(1, m_iScreenSize + 2 + i);
		cout << m_aLog[i];
	}
}

//	게임 틀을 그립니다.
void CGraphic::DrawShape()
{
	string line;
	line.resize(m_iEndOfScreenX * 2 + 2, ' ');
	SetConsoleTextAttribute(m_hOP, TextColor::TEXT_BACKGROUND_WHITE);
	SetCursorPos(0, 0);
	cout << line;
	SetCursorPos(0, m_iScreenSize + 1);
	cout << line;
	SetCursorPos(0, m_iEndOfScreenY);
	cout << line;

	for (int i = 1; i < m_iEndOfScreenY; i++)
	{
		SetCursorPos(0, i);
		cout << m_sPixels[Pixel::blank];
		SetCursorPos(m_iEndOfScreenX, i);
		cout << m_sPixels[Pixel::blank];

		if (i > m_iScreenSize + 1)	continue;
		SetCursorPos(m_iScreenSize + 1, i);
		cout << m_sPixels[Pixel::blank];
	}
}

//	로그 영역을 지웁니다(테두리 제외)
void CGraphic::ClearLog()
{
	string str;
	str.resize((m_iEndOfScreenX - 1) * 2, ' ');

	SetConsoleTextAttribute(m_hOP, TextColor::TEXT_BACKGROUND_BLACK);
	for (int i = m_iScreenSize + 3; i <= m_iScreenSize + m_iMaxLog + 2; i++)
	{
		SetCursorPos(1, i);
		cout << str;
	}
	SetConsoleTextAttribute(m_hOP, TextColor::TEXT_BACKGROUND_BLACK | TextColor::TEXT_FOREGROUND_WHITE);
}
