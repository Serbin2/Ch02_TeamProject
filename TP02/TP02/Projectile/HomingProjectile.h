#pragma once

#include "Projectile.h"

class CHomingProjectile : public CProjectile
{
public:
	CHomingProjectile();
	virtual ~CHomingProjectile() = default;

	virtual std::shared_ptr<CProjectile> Clone() override { return std::make_shared<CHomingProjectile>(*this); }

	virtual void Move() override;
};
