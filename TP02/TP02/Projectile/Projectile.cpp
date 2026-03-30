#include "Projectile.h"

CProjectile::CProjectile(int Shape, int Color) : CActor(Shape, Color)
{
	m_eTag = ETag::actor | ETag::projectile;
}

void CProjectile::Tick(double DeltaTime)
{
	if (!m_bIsValid)
		return;

	if (m_dMoveTimer > 0.0)
		m_dMoveTimer -= DeltaTime;

	if ((m_cMoveDirection.X != 0 || m_cMoveDirection.Y != 0) && m_dMoveTimer <= 0.0)
	{
		Move();
		m_dMoveTimer = 1.0 / m_fSpeed;
	}

	CheckCollision();
}

void CProjectile::Move()
{
	m_cPosition.X += m_cMoveDirection.X;
	m_cPosition.Y += m_cMoveDirection.Y;

	if (m_cPosition.X < 0 || m_cPosition.X >= 30 || m_cPosition.Y < 0 || m_cPosition.Y >= 30)
		m_bIsValid = false;
}

void CProjectile::CheckCollision()
{
	auto pActor = CGameWorld::GetInstance()->FindActorFromPosition(m_cPosition);
	if (!pActor || !pActor->HasTag(ETag::character))
		return;

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
