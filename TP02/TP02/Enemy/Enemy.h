#pragma once
#include "../Character/Character.h"

class CEnemy : public CCharacter
{
protected:
	bool m_bIsDead;
	int m_iExpReward;

public:
	CEnemy(int Shape, int Color);
	~CEnemy() = default;

	virtual void Tick(double DeltaTime) override;
	virtual void Move() override;
	virtual void CheckCollision() override;
	virtual void OnHit(float Damage) override;

	void Die();
	bool IsDead() const { return m_bIsDead; }
};