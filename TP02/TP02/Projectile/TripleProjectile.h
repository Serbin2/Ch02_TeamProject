#pragma once

#include "Projectile.h"

class CTripleProjectile : public CProjectile
{
private:
	bool m_bIsFirstFrame; // 월드에 스폰된 직후 첫 프레임인지 판별

public:
	CTripleProjectile();
	virtual ~CTripleProjectile() = default;

	virtual std::shared_ptr<CProjectile> Clone() { return std::make_shared<CTripleProjectile>(*this); }

	virtual void Tick(double DeltaTime) override;

private:
	void SpawnSideProjectiles();
};