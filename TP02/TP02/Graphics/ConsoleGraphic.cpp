#include "ConsoleGraphic.h"

CGraphic* CGraphic::m_pInstance = nullptr;

CGraphic::CGraphic()
{
	//	콘솔 출력에 대한 기본 핸들을 가져옵니다
	m_hOP = GetStdHandle(STD_OUTPUT_HANDLE);
	SetPixelText();
	SetScreenSize(20, 20);
}

CGraphic::~CGraphic()
{

}

void CGraphic::SetPixelText()
{//	여기서 픽셀의 형태를 정의하세요
	m_sPixels[0] = "  ";
	m_sPixels[1] = "HH";
	m_sPixels[2] = "<>";
	m_sPixels[3] = "()";
	m_sPixels[4] = "--";
	m_sPixels[5] = "||";
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

//	출력 화면의 크기를 설정합니다
//	주의 : 기존의 버퍼를 지웁니다
void CGraphic::SetScreenSize(int x, int y)
{
	m_aBuffer.clear();

	m_aBuffer.resize(y);
	for (auto& i : m_aBuffer)
	{
		i.resize(x, m_sPixels[1]);
	}
	for (auto& i : m_aBuffer[4])
	{
		i = m_sPixels[4];
	}
}

void CGraphic::PrintText(string str)
{
	cout << str;
}

string CGraphic::GetPixelType(Pixel type)
{
	return m_sPixels[type];
}

void CGraphic::Render()
{
	int sizeY = m_aBuffer.size();
	int sizeX = m_aBuffer[0].size();
	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			SetCursorPos(y, x);
			cout << m_aBuffer[x][y];
		}
	}
}