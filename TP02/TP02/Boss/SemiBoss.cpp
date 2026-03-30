#include "SemiBoss.h"
#include "../Graphics/Interface.h"
#include "../Projectile/Projectile.h"
#include "../Character/Player.h"

#define ATTACK_DELAY	5.0
#define SHOT_DELAY 1.0

#define BOSS_HEALTH_UI	19

CSemiBoss::CSemiBoss() : CEnemy(0,0)
{
	m_dMoveTimer = 1.0;
	m_eState = Await;
	m_cRect = COORD(5, 3);
	for (int x = 0; x < m_cRect.X; x++)
	{
		for (int y = 0; y < m_cRect.Y; y++)
		{
			m_iShape[y][x] = Pixel::square;
			m_iColor[y][x] = TEXT_FOREGROUND_BLUE_INT | TEXT_BACKGROUND_BLUE;
		}
	}
	m_iShape[1][1] = m_iShape[1][3] = Pixel::circle;
	m_iColor[1][1] = m_iColor[1][3] = TEXT_FOREGROUND_RED_INT | TEXT_BACKGROUND_GRAY;

	m_cPosition = COORD(12, 0);
	m_ePattern = eVerticalLaser;
	m_dAttackDelay = ATTACK_DELAY;
	m_dShotDelay = SHOT_DELAY;
	m_iAttackCount = 0;
	m_bPatternOn = false;

	m_sName = "[빅헤드]";

	m_fHealth = 1000;
	CInterface::GetInstance()->AddUI(BOSS_HEALTH_UI, "Boss Health ");
	CInterface::GetInstance()->SetValue(BOSS_HEALTH_UI, m_fHealth);
	m_eTag = ETag::actor | ETag::character | ETag::monster | ETag::boss;
}


CSemiBoss::~CSemiBoss()
{

}

void CSemiBoss::Tick(double DeltaTime)
{
	if (!m_bIsValid)	return;

	m_dAttackDelay -= DeltaTime;
	m_dShotDelay -= DeltaTime;
	Attack();

	m_dMoveTimer -= DeltaTime;
	if (m_dMoveTimer > 0)	return;

	Move();
}

void CSemiBoss::Move()
{
	m_dMoveTimer = 1.0;
	switch (m_eState)
	{
	case MovingLeft:
		if (m_cPosition.X == 0)
		{
			m_eState = Await;
			break;
		}
		m_cPosition.X--;
		break;
	case MovingRight:
		if (m_cPosition.X == 25)
		{
			m_eState = Await;
			break;
		}
		m_cPosition.X++;
		break;
	case Await:
		if (m_cPosition.X == 0)
		{
			m_eState = MovingRight;
		}
		else
		{
			m_eState = MovingLeft;
		}
		break;
	}
}

void CSemiBoss::Render()
{
	for (int x = 0; x < m_cRect.X; x++)
	{
		for (int y = 0; y < m_cRect.Y; y++)
		{
			CGraphic::GetInstance()->RenderToBuffer(m_cPosition.X + x, m_cPosition.Y + y, m_iShape[y][x], m_iColor[y][x]);
		}
	}
}

void CSemiBoss::OnHit(float damage)
{
	m_fHealth -= damage;

	CInterface::GetInstance()->SetValue(BOSS_HEALTH_UI, m_fHealth);
	if (m_fHealth <= 0) OnDeath();
}

bool CSemiBoss::ActorCustomCollisionTest(COORD pos)
{	
	if (m_cPosition.X <= pos.X && pos.X <= m_cPosition.X + m_cRect.X &&
		m_cPosition.Y <= pos.Y && pos.Y <= m_cPosition.Y + m_cRect.Y)
	{
		return true;
	}
	return false;
}

void CSemiBoss::Attack()
{
	if (!m_bPatternOn && m_dAttackDelay < 0)
	{	//	패턴이 끝났으니 배정해줍니다
		m_ePattern = rand() % PatternMax;
		m_dShotDelay = SHOT_DELAY;
		m_bPatternOn = true;
	}

	if (!m_bPatternOn)	return;

		//	패턴에 따라 공격 패턴 출력
	switch (m_ePattern)
	{
	case eVerticalLaser:
		VerticalLaser();
		break;
	case eGatling:
		Gatling();
		break;
	}
}

void CSemiBoss::VerticalLaser()
{
	if (m_dShotDelay > 0)	return;

	for (int i = 0; i < 30; i++)
	{
		if (m_iAttackCount % 2 == i % 2)	continue;
		auto pProjectile = std::make_shared<CProjectile>(Pixel::verticalLine, TEXT_FOREGROUND_RED);
		auto sharedOwner = std::static_pointer_cast<CCharacter>(shared_from_this());
		pProjectile->SetOwner(sharedOwner);
		pProjectile->SetPosition(COORD(i,3));
		pProjectile->SetSpeed(5.0f);
		pProjectile->SetMoveDirection(COORD(0,1));
		CGameWorld::GetInstance()->AddActor(pProjectile);
	}
	m_iAttackCount++;
	m_dShotDelay = SHOT_DELAY;

	if (m_iAttackCount > 5)
	{
		m_bPatternOn = false;
		m_dAttackDelay = ATTACK_DELAY;
		m_iAttackCount = 0;
	}
}

void CSemiBoss::Gatling()
{
	if (m_dShotDelay > 0)	return;

	auto pProjectile = std::make_shared<CProjectile>(Pixel::verticalLine, TEXT_FOREGROUND_RED);
	auto sharedOwner = std::static_pointer_cast<CCharacter>(shared_from_this());
	pProjectile->SetOwner(sharedOwner);
	pProjectile->SetPosition(COORD(m_cPosition.X + 2, 3));
	pProjectile->SetSpeed(5.0f);
	pProjectile->SetMoveDirection(COORD(0, 1));
	CGameWorld::GetInstance()->AddActor(pProjectile);

	m_iAttackCount++;
	m_dShotDelay = SHOT_DELAY / 2;

	if (m_iAttackCount > 30)
	{
		m_bPatternOn = false;
		m_dAttackDelay = ATTACK_DELAY;
		m_iAttackCount = 0;
	}
}

void CSemiBoss::OnDeath()
{
	//	공격 강화권 지급하기

	dynamic_pointer_cast<CPlayer>(CGameWorld::GetInstance()->GetPlayerActor())->AddExp(200);

	m_bIsValid = false;
	CInterface::GetInstance()->RemoveUI(BOSS_HEALTH_UI);
}