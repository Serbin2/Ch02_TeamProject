#include "BossProjectile.h"

CBossProjectile::CBossProjectile(int Shape, int Color, int LifeTime)
	: CProjectile(Shape, Color)
{
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
		m_bIsActive = false;
		m_bIsValid = false;
		return;
	}

	CProjectile::Tick(DeltaTime);
}
