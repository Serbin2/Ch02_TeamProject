#pragma once
#include "Enemy.h"


//	몬스터 예시 및 테스트용 슬라임 클래스입니다.
class CSlime : public CEnemy
{
public:
	CSlime();
	~CSlime();

	virtual void Tick(double DeltaTime) override;
	virtual void Move() override;
	virtual void OnHit(float Damage)	override;

protected:

	double m_dAnimationTime;
	int m_iShapeA;
	int m_iShapeB;
	int m_iShapeHitted;
	bool m_bShape;

};