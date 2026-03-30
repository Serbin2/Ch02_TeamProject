#pragma once
#include "../Character/Character.h"

class CPlayer;

class CEnemy : public CCharacter
{
protected:
	CPlayer* player = nullptr;
	int m_iExpReward;

public:
	CEnemy(int Shape, int Color);
	~CEnemy() = default;

	virtual void Tick(double DeltaTime) override;
	virtual void Move() override;
	virtual void Attack(COORD Direction) override;
	virtual void OnHit(float Damage) override;

	virtual void Die();
	virtual void DropReward();
	virtual void SetPlayer(CPlayer* InPlayer);

};
