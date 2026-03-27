#include "Projectile.h"

CProjectile::CProjectile(int Shape, int Color) : CActor(Shape, Color)
{
	m_bIsActive = true;
	m_eTag = ETag::actor | ETag::projectile;
}

void CProjectile::Tick(double DeltaTime)
{
	if (!m_bIsValid)	return;

	if (m_dMoveTimer > 0.0)
	{
		m_dMoveTimer -= DeltaTime;
	}

	if ((m_cMoveDirection.X != 0 || m_cMoveDirection.Y != 0) && m_dMoveTimer <= 0.0)
	{
		Move();
		m_dMoveTimer = 1.0 / m_fSpeed;
	}

	if (!m_bIsActive)
	{
		return;
	}
}

void CProjectile::Move()
{
	m_cPosition.X += m_cMoveDirection.X;
	m_cPosition.Y += m_cMoveDirection.Y;

	if (m_cPosition.X < 0 || m_cPosition.X >= 30 || m_cPosition.Y < 0 || m_cPosition.Y >= 30)
		m_bIsActive = false;
}
