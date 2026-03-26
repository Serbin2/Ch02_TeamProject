#pragma once

#include "../Standard.h"
#include "../Graphics/ConsoleGraphic.h"

class CActor
{
protected:
	virtual	~CActor() = default;
public:
	CActor() = default;

	CActor(int Shape, int Color) : m_pShape(Shape), m_tColor(Color) { }

	

	virtual void Tick(double DeltaTime) = 0;
	virtual void Move() = 0;

	virtual void Render()
	{
		CGraphic* pGraphic = CGraphic::GetInstance();
		if (pGraphic)
		{
			pGraphic->RenderToBuffer(m_cPosition.X, m_cPosition.Y, m_pShape, m_tColor);
		}
	}

	friend class CGameWorld;

	const COORD GetPosition() const { return m_cPosition; }
	void SetPosition(const COORD& NewPosition) { m_cPosition = NewPosition; }

	const COORD GetMoveDirection() const { return m_cMoveDirection; }
	void SetMoveDirection(const COORD& NewDirection) { m_cMoveDirection = NewDirection; }

	const float GetSpeed() const { return m_fSpeed; }
	void SetSpeed(float NewSpeed) { m_fSpeed = NewSpeed; }

protected:
	int m_pShape;	        // 액터의 픽셀 모양
	int m_tColor;           // 액터의 텍스트 색상

	COORD m_cPosition;		// 액터의 현재 위치
	COORD m_cMoveDirection; // 액터의 이동 방향
	float m_fSpeed;			// 액터의 이동 속도(m/s)
	double m_dMoveTimer;	// 액터의 이동 타이머

	bool m_bIsValid = true;
};
