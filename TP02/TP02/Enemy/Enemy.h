#pragma once

#include "../Character/Character.h"

class CEnemy : public CCharacter
{
public:
	CEnemy(int Shape, int Color);
	~CEnemy() = default;

	virtual void Tick(double DeltaTime) override;
	virtual void Move() override;
	virtual void Attack(COORD Direction) override;
	virtual void OnHit(float Damage) override;
};
