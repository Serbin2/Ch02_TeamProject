#pragma once

#include "Character.h"

class CPlayer : public CCharacter
{
public:
	CPlayer(int Shape, int Color);
	virtual ~CPlayer() = default;

	virtual void Tick(double DeltaTime) override;
	virtual void Move() override;
	virtual void Attack(COORD Direction) override;
	virtual void OnHit(float Damage) override;

private:
	void Input();

private:
	int m_iLevel;
	int m_iExp;

	// PJH - 인벤토리 추가
	std::shared_ptr<class CInventory> m_pInventory;
};
