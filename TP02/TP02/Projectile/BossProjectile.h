// [작성자] : 박재현
// [Desc] : 라이프타임을 가지는 프로젝타임

#pragma once
#include "Projectile.h"

class CBossProjectile : public CProjectile
{
public:
	CBossProjectile(int Shape, int Color, double LifeTime);

	virtual	~CBossProjectile();

public:
	void Tick(double DeltaTime) override;


private:
	double m_dLifeTime = 0.5;
};

