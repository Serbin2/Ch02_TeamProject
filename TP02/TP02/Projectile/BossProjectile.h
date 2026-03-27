#pragma once
#include "Projectile.h"

class CBossProjectile : public CProjectile
{
public:
	CBossProjectile(int Shape, int Color, COORD MoveDirection, int LifeTime);

	virtual	~CBossProjectile();


private:
	double m_dLifeTime = 0.5;
};

