#pragma once

#include "Projectile.h"

class CBouncingProjectile : public CProjectile
{
public:
	CBouncingProjectile();
	virtual ~CBouncingProjectile() = default;

	virtual std::shared_ptr<CProjectile> Clone() { return std::make_shared<CBouncingProjectile>(*this); }

	virtual void Move() override;

protected:
	int m_iBounceCount; // 남은 튕김 횟수

};
