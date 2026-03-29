#pragma once

#include "../Player.h"

//	플레이어 테스트용 스나이퍼 클래스입니다.
class CSniper : public CPlayer
{
public:
	CSniper();
	virtual ~CSniper();

	virtual void Tick(double DeltaTime) override;
	virtual void Move() override;
	virtual void Attack(COORD Direction) override;
	virtual void OnHit(float Damage) override;
	virtual void Render() override;

protected:

	double m_dShotDelay;			//	기본 공격 딜레이
	double m_dInvincibleTime;		//	피격시 무적시간
	bool m_bInvincible;				//	현재 무적인지

	vector<pair<int, int>> m_vShotMaterial;		//	공격 이펙트
	vector<double> m_vShotDuration;				//	공격 이펙트
};