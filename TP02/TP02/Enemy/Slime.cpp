#include "Slime.h"
#include "../Manager/SoundManager/SoundManager.h"


CSlime::CSlime() : CEnemy(Pixel::circle, TEXT_FOREGROUND_BLUE_INT | TEXT_BACKGROUND_GREEN_INT)
{
	//	무작위 위치에 생성합니다.
	m_cPosition.X = (rand() % 28) + 1;
	m_cPosition.Y = (rand() % 28) + 1;

	//	애니메이션 세팅입니다.
	m_dAnimationTime = (float)rand() / RAND_MAX;	//	0 ~ 1 사이의 실수 생성
	m_iShapeA = Pixel::circle;
	m_iShapeB = Pixel::horizontalLine;
	m_iShapeHitted = Pixel::star;
	m_tColor = TEXT_FOREGROUND_BLUE_INT | TEXT_BACKGROUND_GREEN_INT;
	m_bShape = true;

	//	기본 정보를 설정합니다.
	m_sName = "고철수집기";
	m_eTag = ETag::actor | ETag::character | ETag::monster;
	m_pShape = m_iShapeA;
	m_fAttackPower = 10.0f;
	m_fDefense = 3.0f;

	GET_SINGLE(CSoundManager)->PlaySFX(L"Slime");
}


CSlime::~CSlime()
{
	GET_SINGLE(CSoundManager)->PlaySFX(L"SlimeDead");
}

void CSlime::Tick(double DeltaTime)
{
	//	기본 처리를 합니다.
	CEnemy::Tick(DeltaTime);
	if (!m_bIsValid)	return;

	if (m_fHealth < 1)
	{	//	체력이 없으면 삭제
		m_bIsValid = false;
		return;
	}

	//	애니메이션 처리입니다
	m_dAnimationTime -= DeltaTime;
	if (m_dAnimationTime > 0)	return;

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
}

void CSlime::OnHit(float damage)
{
	CEnemy::OnHit(damage);

	if (m_fHealth <= 0.0f) DropItem();

	m_pShape = m_iShapeHitted;
	m_dAnimationTime = 1.0;
}

