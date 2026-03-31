#pragma once

#include "Actor.h"
#include "../World/GameWorld.h"

class CCharacter : public CActor
{
public:
	CCharacter(int Shape, int Color) : CActor(Shape, Color) { m_eTag = ETag::actor | ETag::character; }
	virtual ~CCharacter() = default;

	float GetHealth() const { return m_fHealth; }
	void SetHealth(float NewHealth);

	float GetMaxHealth() const { return m_fMaxHealth; }
	void SetMaxHealth(float NewMaxHealth);

	virtual void Attack(COORD Direction) { return; }

	const float GetAttackPower() const { return m_fAttackPower; }
	void SetAttackPower(float NewAttackPower) { m_fAttackPower = NewAttackPower; }

protected:
	float m_fHealth;		  // 체력
	float m_fMaxHealth;		  // 최대체력
	float m_fDefense;		  // 방어력
	float m_fAttackPower;	  // 공격력
	int m_iAttackRange;		  // 공격 사거리
	double m_dAttackCooldown; // 공격 쿨타임
	double m_dAttackTimer;	  // 다음 공격까지 남은 시간
};
