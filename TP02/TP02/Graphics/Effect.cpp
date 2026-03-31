#include "Effect.h"

CEffect::CEffect()
{
	m_cRectLT = { 0, };
	m_cRectRB = { 0, };
	m_dDuration = 0.0;

	m_bDynamic = false;
	m_iDynamicLevel = 0;
	m_iDynamicCurLevel = 0;

	m_eTag = ETag::actor | ETag::effect;
	m_isFired = false;
}

CEffect::~CEffect()
{
	
}

void CEffect::CreateStaticEffect(int shape,	int color, COORD LT, COORD RB, double duration )
{
	m_pShape = shape;
	m_tColor = color;
	//	그리기 영역에 맞게 보정
	m_cRectLT.X = max(0, min(LT.X, RB.X));
	m_cRectLT.Y = max(0, min(LT.Y, RB.Y));
	m_cRectRB.X = min(29, max(LT.X, RB.X));
	m_cRectRB.Y = min(29, max(LT.Y, RB.Y));
	m_dDuration = duration;
	m_isFired = true;
}

void CEffect::CreateDynamicEffect(int level, vector<pair<int, int>> shape, vector<pair<COORD,COORD>> rect, vector<double> duration)
{
	m_iDynamicLevel = level;
	m_aDynamicMaterial = shape;
	m_aDynamicShape = rect;
	m_aDynamicDuration = duration;

	m_aDynamicShape.resize(level);
	m_aDynamicMaterial.resize(level);
	m_aDynamicDuration.resize(level);

	for (auto& i : m_aDynamicShape)
	{
		COORD LT;
		COORD RB;
		LT.X = max(0, min(i.first.X, i.second.X));
		LT.Y = max(0, min(i.first.Y, i.second.Y));
		RB.X = min(29, max(i.first.X, i.second.X));
		RB.Y = min(29, max(i.first.Y, i.second.Y));
		i.first = LT;
		i.second = RB;
	}

	m_bDynamic = true;
	m_iDynamicCurLevel = -1;
	m_dDuration = 0;
	m_isFired = true;
}

void CEffect::Tick(double deltaTime)
{
	if (!m_bIsValid)	return;
	if (!m_isFired)	return;

	m_dDuration -= deltaTime;

	if (m_bDynamic)	DynamicEffectTick(deltaTime);

	if (m_dDuration > 0)	return;

	//	이펙트 지속시간이 끝났습니다.
	m_bIsValid = false;
}

void CEffect::Move()
{	//	이펙트는 움직이지 않도록 합니다.
	//	특수한 이펙트를 만들려면 자식 클래스를 생성하여 재정의 해주세요
	return;
}

void CEffect::Render()
{
	if (!m_bIsValid)	return;
	if (m_iDynamicCurLevel < 0)	return;	//	다이나믹 이펙트로 생성했으나 아직 Tick을 타지 않은 상태 : 터질 수 있음

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

void CEffect::DynamicEffectTick(double deltaTime)
{	//	다이나믹 이펙트를 다음 레벨로 진행시킵니다.
	if (m_dDuration > 0)	return;
	
	//	레벨을 증가시킵니다.
	m_iDynamicCurLevel++;

	//	최대 레벨을 넘었으면 비활성화합니다.
	if (m_iDynamicCurLevel >= m_iDynamicLevel)
	{	//	CurLevel은 인덱스를 사용하므로 0부터시작
		m_bIsValid = false;
		m_isFired = false;
		return;
	}
	
	m_pShape = m_aDynamicMaterial[m_iDynamicCurLevel].first;
	m_tColor = m_aDynamicMaterial[m_iDynamicCurLevel].second;
	m_cRectLT = m_aDynamicShape[m_iDynamicCurLevel].first;
	m_cRectRB = m_aDynamicShape[m_iDynamicCurLevel].second;
	m_dDuration = m_aDynamicDuration[m_iDynamicCurLevel];
}