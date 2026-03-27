#include "BossProjectile.h"

CBossProjectile::CBossProjectile(int Shape, int Color, COORD MoveDirection, int LifeTime)
	: CProjectile(Shape, Color)
{
	SetMoveDirection(MoveDirection);
	m_dLifeTime = LifeTime;
}

CBossProjectile::~CBossProjectile()
{
}

void CBossProjectile::Tick(double DeltaTime)
{
	m_dLifeTime -= DeltaTime;
	if (m_dLifeTime <= 0)
	{
		// 랜더 종료 
		m_bIsValid = false;
		return;
	}


	CProjectile::Tick(DeltaTime);
}
