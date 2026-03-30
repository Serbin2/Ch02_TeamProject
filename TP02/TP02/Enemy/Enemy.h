#pragma once
#include "../Character/Character.h"

class CPlayer;

class CEnemy : public CCharacter
{
private:
	int m_iExpReward;
	CPlayer* player = nullptr;

public:
	CEnemy(int Shape, int Color);
	~CEnemy() = default;

	virtual void Tick(double DeltaTime) override;
	virtual void Move() override;
	virtual void Attack(COORD Direction) override;
	virtual void OnHit(float Damage) override;

	void Die();
	void DropReward();
	void SetPlayer(CPlayer* InPlayer);

};
