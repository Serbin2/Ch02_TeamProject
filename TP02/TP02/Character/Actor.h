#pragma once

#include "../Standard.h"
#include "../Graphics/ConsoleGraphic.h"

class CActor
{
public:
	CActor() = default;

	CActor(int Shape, int Color) : m_pShape(Shape), m_tColor(Color) { }

	virtual	~CActor() = default;

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

	const COORD GetPosition() const { return m_cPosition; }
	void SetPosition(const COORD& NewPosition) { m_cPosition = NewPosition; }

protected:
	int m_pShape;	        // 액터의 픽셀 모양
	int m_tColor;           // 액터의 텍스트 색상

	COORD m_cPosition;		// 액터의 현재 위치
	float m_fSpeed;			// 액터의 이동 속도(m/s)
	double m_dMoveTimer;	// 액터의 이동 타이머

};
