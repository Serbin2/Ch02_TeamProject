#pragma once

#include "../Enemy.h"

class CSkeleton : public CEnemy
{	//	일정시간 이동 후 원거리 공격하는 스켈레톤 몬스터 만들기
public:
	CSkeleton();
	~CSkeleton();

	virtual void Tick(double deltaTime) override;
	virtual void Move() override;

	virtual void DropItem();


protected:

	enum State
	{
		Moving,
		WaitForShot,
		WaitForMove,
	};

	void Shot();

	int m_iState;
	double m_dStateTime;

};
