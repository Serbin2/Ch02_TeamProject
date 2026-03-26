#pragma once

#include <windows.h>

class CActor
{
public:
	CActor() = default;

	virtual	~CActor() = default;

	virtual void Tick(float DeltaTime) = 0;
	virtual void Move() = 0;
	virtual void Attack() = 0;
	virtual void OnHit(float Damage) = 0;

	const Coord GetPosition() const { return m_cPosition; }
	void SetPosition(const Coord& NewPosition) { m_cPosition = NewPosition; }

private:
	Coord m_cPosition;		// 현재 위치
	float m_fSpeed;			// 이동 속도
	float m_fHealth;		// 체력
	float m_fDefense;		// 방어력
	float m_fAttackPower;	// 공격력
	int m_iAttackRange;		// 공격 사거리
	float mAttackCooldown;	// 공격 쿨타임

};