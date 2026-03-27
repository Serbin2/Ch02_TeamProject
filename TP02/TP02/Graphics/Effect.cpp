#include "Effect.h"

CEffect::CEffect()
{
	m_cRectLT = { 0, };
	m_cRectRB = { 0, };
	m_dDuration = 0.0;

	m_eTag = ETag::actor | ETag::effect;
	m_isFired = false;
}

CEffect::~CEffect()
{
	
}

void CEffect::Create(int shape,	int color, COORD LT, COORD RB, double duration )
{
	m_pShape = shape;
	m_tColor = color;
	//	그리기 영역에 맞게 보정
	m_cRectLT.X = max(0, LT.X);
	m_cRectLT.Y = max(0, LT.Y);
	m_cRectRB.X = min(29, RB.X);
	m_cRectRB.Y = min(29, RB.Y);
	m_dDuration = duration;

	CGraphic::GetInstance()->AddLog("이펙트를 생성했습니다");
	m_isFired = true;
}

void CEffect::Tick(double deltaTime)
{
	if (!m_bIsValid)	return;
	if (!m_isFired)	return;

	m_dDuration -= deltaTime;

	if (m_dDuration > 0)	return;

	//	이펙트 지속시간이 끝났습니다.
	m_bIsValid = false;
	CGraphic::GetInstance()->AddLog("이펙트가 삭제되었습니다");
}

void CEffect::Move()
{	//	이펙트는 움직이지 않도록 합니다.
	//	특수한 이펙트를 만들려면 자식 클래스를 생성하여 재정의 해주세요
	return;
}

void CEffect::Render()
{
	if (!m_bIsValid)	return;

	//	렉트 크기만큼 그립니다
	CGraphic* pGraphic = CGraphic::GetInstance();

	for (int x = m_cRectLT.X; x <= m_cRectRB.X; x++)
	{
		for (int y = m_cRectLT.Y; y <= m_cRectRB.Y; y++)
		{
			pGraphic->RenderToBuffer(x, y, m_pShape, m_tColor);
		}
	}
}