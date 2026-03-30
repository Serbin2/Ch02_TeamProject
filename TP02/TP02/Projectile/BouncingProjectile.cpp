#include "BouncingProjectile.h"

CBouncingProjectile::CBouncingProjectile() : CProjectile(Pixel::circle, TEXT_FOREGROUND_GREEN)
{
	m_iBounceCount = 3; // 기본적으로 3번 튕김
}

void CBouncingProjectile::Move()
{
	if ((m_cMoveDirection.X == 0 && m_cMoveDirection.Y == 0) || m_dMoveTimer > 0.0)
		return;

	short nextX = m_cPosition.X + m_cMoveDirection.X;
	short nextY = m_cPosition.Y + m_cMoveDirection.Y;

	// X축 경계 검사 및 반사
	if (nextX < 0 || nextX >= 30)
	{
		if (m_iBounceCount > 0)
		{
			m_cMoveDirection.X *= -1;
			m_iBounceCount--;
			nextX = m_cPosition.X + m_cMoveDirection.X;
		}
		else
		{
			m_bIsValid = false;
			return;
		}
	}

	// Y축 경계 검사 및 반사
	if (nextY < 0 || nextY >= 30)
	{
		if (m_iBounceCount > 0)
		{
			m_cMoveDirection.Y *= -1;
			m_iBounceCount--;
			nextY = m_cPosition.Y + m_cMoveDirection.Y;
		}
		else
		{
			m_bIsValid = false;
			return;
		}
	}

	m_cPosition.X = nextX;
	m_cPosition.Y = nextY;
	m_dMoveTimer = 1.0 / m_fSpeed;
}
