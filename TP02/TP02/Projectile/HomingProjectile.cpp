#include "HomingProjectile.h"
#include <cmath>

CHomingProjectile::CHomingProjectile() : CProjectile(Pixel::circle, TEXT_FOREGROUND_GRAY) { }

void CHomingProjectile::Move()
{
	if (m_dMoveTimer > 0.0)
		return;

	auto pOwner = m_pOwner.lock();
	int targetX = -1;
	int targetY = -1;
	double minDistance = 9999.0;

	// 맵 전체를 탐색하여 가장 가까운 타겟 찾기
	for (int y = 0; y < 30; ++y)
	{
		for (int x = 0; x < 30; ++x)
		{
			COORD pos = { (short)x, (short)y };
			auto pActor = CGameWorld::GetInstance()->FindActorFromPosition(pos);

			if (pActor == nullptr || !pActor->HasTag(ETag::character))
				continue;

			if (pOwner && pOwner.get() == pActor.get())
				continue;

			double dist = std::sqrt(std::pow(m_cPosition.X - x, 2) + std::pow(m_cPosition.Y - y, 2));

			if (dist < minDistance)
			{
				minDistance = dist;
				targetX = x;
				targetY = y;
			}
		}
	}

	// 타겟 방향으로 m_cMoveDirection 갱신
	if (targetX != -1 && targetY != -1)
	{
		int diffX = targetX - m_cPosition.X;
		int diffY = targetY - m_cPosition.Y;

		m_cMoveDirection = { 0, 0 };
		if (diffX != 0) m_cMoveDirection.X = (diffX > 0) ? 1 : -1;
		if (diffY != 0) m_cMoveDirection.Y = (diffY > 0) ? 1 : -1;
	}
	else if (m_cMoveDirection.X == 0 && m_cMoveDirection.Y == 0)
	{
		return;
	}

	// 이동 및 경계 검사
	short nextX = m_cPosition.X + m_cMoveDirection.X;
	short nextY = m_cPosition.Y + m_cMoveDirection.Y;

	if (nextX < 0 || nextX >= 30 || nextY < 0 || nextY >= 30)
	{
		m_bIsValid = false;
		return;
	}

	m_cPosition.X = nextX;
	m_cPosition.Y = nextY;
	m_dMoveTimer = 1.0 / m_fSpeed;
}
