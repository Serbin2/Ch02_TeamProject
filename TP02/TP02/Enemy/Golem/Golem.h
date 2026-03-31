#pragma once

#include "../Enemy.h"

class CGolem : public CEnemy
{	//	움직일때마다 범위공격하는 골렘 만들기
public:
	CGolem();
	~CGolem();

	virtual void Tick(double deltaTime) override;


protected:
	void Attack();

	vector<pair<int, int >> m_aMaterial;
	vector<double> m_aDuration;
};