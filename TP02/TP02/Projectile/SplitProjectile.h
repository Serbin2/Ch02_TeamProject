#pragma once

#include "Projectile.h"

class CSplitProjectile : public CProjectile
{
public:
	CSplitProjectile();
	virtual ~CSplitProjectile() = default;

	virtual std::shared_ptr<CProjectile> Clone() override { return std::make_shared<CSplitProjectile>(*this); }

	virtual void CheckCollision() override;
};
