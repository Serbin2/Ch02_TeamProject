#pragma once

#include "../Standard.h"
#include "../Graphics/ConsoleGraphic.h"

class CActor
{
public:
	CActor() = default;

	CActor(int Shape, int Color) : m_pShape(Shape), m_tColor(Color), m_eTag(ETag::actor) { }
	virtual	~CActor() = default;
	

	virtual void Tick(double DeltaTime) = 0;
	virtual void Move() = 0;

	virtual void Render()
	{
		if (!m_bIsValid)	return;
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
	
	bool HasTag(ETag tag)
	{
		return m_eTag & tag;
	}

	void AddTag(ETag tag)
	{
		m_eTag = m_eTag | tag;
	}

	void RemoveTag(ETag tag)
	{
		m_eTag = m_eTag & ~tag;
	}

protected:
	int m_pShape;	        // 액터의 픽셀 모양
	int m_tColor;           // 액터의 텍스트 색상

	COORD m_cPosition;		// 액터의 현재 위치
	COORD m_cMoveDirection; // 액터의 이동 방향
	float m_fSpeed;			// 액터의 이동 속도(m/s)
	double m_dMoveTimer;	// 액터의 이동 타이머

	int m_eTag;
	bool m_bIsValid = true;
};
