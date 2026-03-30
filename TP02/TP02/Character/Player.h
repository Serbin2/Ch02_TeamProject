#pragma once
#include "Character.h"
#include <vector>
#include <memory>

class CPlayer : public CCharacter
{
public:
	CPlayer(int Shape, int Color);
	virtual ~CPlayer() = default;

	virtual void Tick(double DeltaTime) override;
	virtual void Move() override;
	virtual void Attack(COORD Direction) override;
	virtual void OnHit(float Damage) override;

public:
	std::shared_ptr<class CInventory> GetInventory() const;

private:
	void Input();

private:
	// PJH - 인벤토리 추가
	std::shared_ptr<class CInventory> m_pInventory;

	int m_iLevel;
	int m_iExp;
	// 골드 (상점 시스템용)
	int m_iGold = 500;


public:
	// 골드
	int  GetGold() const { return m_iGold; }
	void AddGold(int amount) { m_iGold += amount; }
	bool SpendGold(int amount)
	{
		if (m_iGold < amount) return false;
		m_iGold -= amount;
		return true;
	}
};
