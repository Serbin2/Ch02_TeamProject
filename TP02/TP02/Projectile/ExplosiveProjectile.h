#pragma once

#include "Projectile.h"

class CExplosiveProjectile : public CProjectile
{
public:
	CExplosiveProjectile();
	virtual ~CExplosiveProjectile() = default;

	virtual std::shared_ptr<CProjectile> Clone() override { return std::make_shared<CExplosiveProjectile>(*this); }

	virtual void CheckCollision() override;
};
