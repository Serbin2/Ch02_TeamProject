#include "Slime.h"
#include <cstdlib>

CSlime::CSlime() : CEnemy(Pixel::circle, TEXT_FOREGROUND_BLUE_INT | TEXT_BACKGROUND_GREEN_INT)
{
	m_cPosition.X = (rand() % 28) + 1;
	m_cPosition.Y = (rand() % 28) + 1;

	m_dAnimationTime = (float)rand() / RAND_MAX;
	m_iShapeA = Pixel::circle;
	m_iShapeB = Pixel::horizontalLine;
	m_iShapeHitted = Pixel::star;
	m_tColor = TEXT_FOREGROUND_BLUE_INT | TEXT_BACKGROUND_GREEN_INT;
	m_bShape = true;

	m_sName = "슬라임";
	m_pShape = m_iShapeA;
}

CSlime::~CSlime()
{
}

void CSlime::Tick(double DeltaTime)
{
	CEnemy::Tick(DeltaTime);
	if (!m_bIsValid) return;

	m_dAnimationTime -= DeltaTime;
	if (m_dAnimationTime > 0.0) return;

	if (m_bShape)
	{
		m_pShape = m_iShapeB;
		m_bShape = false;
	}
	else
	{
		m_pShape = m_iShapeA;
		m_bShape = true;
	}

	m_dAnimationTime = 1.0;

<<<<<<< HEAD
	CheckCollision();
=======
>>>>>>> PDH02
}

void CSlime::Move()
{
	// 테스트용: 움직이지 않음
}

void CSlime::OnHit(float Damage)
{
    CEnemy::OnHit(Damage);
    if (m_bIsDead) return;

    m_pShape = m_iShapeHitted;
    m_dAnimationTime = 1.0;
}
