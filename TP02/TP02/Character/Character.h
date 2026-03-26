#pragma once

#include "Actor.h"

class CCharacter : public CActor
{
public:
	CCharacter(int Shape, int Color) : CActor(Shape, Color) { }

	virtual void Attack(COORD Direction) = 0;
	virtual void OnHit(float Damage) = 0;

	const float GetAttackPower() const { return m_fAttackPower; }
	void SetAttackPower(float NewAttackPower) { m_fAttackPower = NewAttackPower; }

protected:
	float m_fHealth;	     // 체력
	float m_fDefense;	     // 방어력
	float m_fAttackPower;    // 공격력
	int m_iAttackRange;	     // 공격 사거리
	float m_fAttackCooldown; // 공격 쿨타임

};
