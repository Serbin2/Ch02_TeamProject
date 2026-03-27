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

