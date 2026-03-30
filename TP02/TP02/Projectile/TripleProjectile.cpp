#include "TripleProjectile.h"

CTripleProjectile::CTripleProjectile() : CProjectile(Pixel::circle, TEXT_FOREGROUND_BLUE)
{
	m_bIsFirstFrame = true;
}

void CTripleProjectile::Tick(double DeltaTime)
{
	if (!m_bIsValid)
		return;

	// 월드에 등록되고 처음 실행되는 틱(Tick)에서만 양옆 총알 생성
	if (m_bIsFirstFrame)
	{
		m_bIsFirstFrame = false;
		SpawnSideProjectiles();
	}

	CProjectile::Tick(DeltaTime);
}

void CTripleProjectile::SpawnSideProjectiles()
{
	COORD dir2 = m_cMoveDirection;
	COORD dir3 = m_cMoveDirection;

	// X축 직진 중일 때 위아래로 꺾음
	if (m_cMoveDirection.X != 0)
	{
		dir2.Y = -1;
		dir3.Y = 1;
	}
	// Y축 직진 중일 때 좌우로 꺾음
	else if (m_cMoveDirection.Y != 0)
	{
		dir2.X = -1;
		dir3.X = 1;
	}

	COORD directions[2] = { dir2, dir3 };

	for (int i = 0; i < 2; ++i)
	{
		// 무한 분열을 막기 위해 양옆 총알은 자식 클래스가 아닌 '기본 부모 클래스(CProjectile)'로 생성
		auto pProjectile = std::make_shared<CProjectile>(Pixel::circle, TEXT_FOREGROUND_BLUE);
		pProjectile->SetOwner(m_pOwner);
		pProjectile->SetPosition(m_cPosition);
		pProjectile->SetMoveDirection(directions[i]);

		CGameWorld::GetInstance()->AddActor(pProjectile);
	}
}
