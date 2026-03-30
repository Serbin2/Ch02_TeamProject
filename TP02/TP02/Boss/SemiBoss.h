#pragma once

#include "../Character/Actor.h"

class CSemiBoss : public CActor
{
public:
	CSemiBoss();
	~CSemiBoss();

	virtual void Tick(double DeltaTime) override;
	virtual void Move() override;
	virtual void Render() override;

protected:
	enum State
	{	//	보스의 상태
		MovingRight,
		MovingLeft,
		Await,

	};

	//int m_pShape;	        // 액터의 픽셀 모양
	//int m_tColor;           // 액터의 텍스트 색상
	//
	//COORD m_cPosition;		// 액터의 현재 위치
	//COORD m_cMoveDirection; // 액터의 이동 방향
	//float m_fSpeed;			// 액터의 이동 속도(m/s)
	//double m_dMoveTimer;	// 액터의 이동 타이머

	//	거대하 사각형 형태의 보스
	COORD m_cRect;
	int m_iTexture[3][5];

	State m_eState;
};