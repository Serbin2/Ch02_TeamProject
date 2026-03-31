#pragma once

#include "../Enemy/Enemy.h"

class CSemiBoss : public CEnemy
{
public:
	CSemiBoss();
	~CSemiBoss();

	virtual void Tick(double DeltaTime) override;
	virtual void Move() override;
	virtual void Render() override;

	virtual void OnHit(float daamge) override;
	virtual bool ActorCustomCollisionTest(COORD pos) override;

	virtual void Die() override;

protected:
	enum State
	{	//	보스의 상태
		MovingRight,
		MovingLeft,
		Await,
	};

	enum AttackPattern
	{
		eVerticalLaser,
		eGatling,


		PatternMax,
	};

	void Attack();
	void VerticalLaser();
	void Gatling();

	int m_ePattern;
	double m_dAttackDelay;	//	공격 패턴 전환 딜레이
	double m_dShotDelay;	//	공격 발사 딜레이
	double m_dAccel;		//	가속
	int m_iAttackCount;
	bool m_bPatternOn;

	//	거대한 사각형 형태의 보스
	COORD m_cRect;
	int m_iShape[3][5];
	int m_iColor[3][5];

	State m_eState;
};