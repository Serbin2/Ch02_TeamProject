#include "Projectile.h"

CProjectile::CProjectile(int Shape, int Color) : CActor(Shape, Color)
{
	m_fSpeed = 5.0f;
	m_dLifeTime = 20.0f;	//	모든 투사체는 20초 후에 강제 제거
	m_eTag = ETag::actor | ETag::projectile;
}

void CProjectile::Tick(double DeltaTime)
{
	if (!m_bIsValid)
		return;

	if (m_dMoveTimer > 0.0) m_dMoveTimer -= DeltaTime;
	m_dLifeTime -= DeltaTime;

	if (m_dLifeTime < 0)	m_bIsValid = false;

	Move();
	CheckCollision();
}

void CProjectile::Move()
{
	if ((m_cMoveDirection.X == 0 && m_cMoveDirection.Y == 0) || m_dMoveTimer > 0.0)
		return;

	m_cPosition.X += m_cMoveDirection.X;
	m_cPosition.Y += m_cMoveDirection.Y;

	if (m_cPosition.X < 0 || m_cPosition.X >= 30 || m_cPosition.Y < 0 || m_cPosition.Y >= 30)
		m_bIsValid = false;

	m_dMoveTimer = 1.0 / m_fSpeed;
}

void CProjectile::CheckCollision()
{
	int findingTag = ETag::player;
	if (m_pOwner.lock() == CGameWorld::GetInstance()->GetPlayerActor())
	{
		findingTag = ETag::monster;
	}
	auto pActor = CGameWorld::GetInstance()->FindActorFromPosition(m_cPosition, findingTag);
	if (!pActor)	pActor = CGameWorld::GetInstance()->FindActorByActorCustom(m_cPosition , findingTag);

	auto pCharacter = std::dynamic_pointer_cast<CCharacter>(pActor);
	if (!pCharacter)
		return;

	auto pOwner = m_pOwner.lock();
	if (pOwner && pOwner.get() != pCharacter.get())
	{
		pCharacter->OnHit(pOwner->GetAttackPower());
#ifdef _DEBUG
		string str = pOwner.get()->GetName() + "의 투사체가 " + pCharacter.get()->GetName() + "에게 충돌했습니다.";
		CGraphic::GetInstance()->AddLog(str);
#endif
		m_bIsValid = false;
	}
}
