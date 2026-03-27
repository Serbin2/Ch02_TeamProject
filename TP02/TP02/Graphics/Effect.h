#pragma once

#include "ConsoleGraphic.h"
#include "../Character/Actor.h"

class CEffect : public CActor
{
public:
	CEffect();
	~CEffect();

	//	이펙트를 생성한 후 호출하여 필요한 정보들을 정의해주세요
	void Create(
		int shape,			//	이펙트의 모양입니다
		int color,			//	이펙트의 색상입니다
		COORD LT,			//	사각형의 왼쪽 위 지점
		COORD RB,			//	사각형의 오른쪽 아래 지점
		double duration		//	지속시간
		);

	virtual void Tick(double deltaTime);
	virtual void Move();

	virtual void Render();


protected:

	//int m_pShape;	        // 액터의 픽셀 모양
	//int m_tColor;           // 액터의 텍스트 색상
	//
	//COORD m_cPosition;		// 액터의 현재 위치
	//COORD m_cMoveDirection; // 액터의 이동 방향
	//float m_fSpeed;			// 액터의 이동 속도(m/s)
	//double m_dMoveTimer;	// 액터의 이동 타이머

	//	사각형 영역입니다.
	COORD m_cRectLT;
	COORD m_cRectRB;

	//	지속시간
	double m_dDuration;
	bool m_isFired;			//	초기화가 있기 전까진 실행하지 않도록 합니다.
};