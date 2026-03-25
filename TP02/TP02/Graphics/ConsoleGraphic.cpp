#include "ConsoleGraphic.h"

CGraphic* CGraphic::m_pInstance = nullptr;

CGraphic::CGraphic()
{
	//	콘솔 출력에 대한 기본 핸들을 가져옵니다
	m_hOP = GetStdHandle(STD_OUTPUT_HANDLE);
}

CGraphic::~CGraphic()
{

}

CGraphic* CGraphic::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CGraphic();
	}

	return m_pInstance;
}