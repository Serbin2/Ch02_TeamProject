
#include "SemiBoss.h"

CSemiBoss::CSemiBoss()
{
	m_dMoveTimer = 1.0;
	m_eState = Await;
	m_cRect = COORD(5, 3);
	for (auto& i : m_iTexture)
	{
		for (auto& j : i)
		{
			j = Pixel::square;
		}
	}
	m_iTexture[1][1] = m_iTexture[1][3] = Pixel::circle;

	m_cPosition = COORD(12, 0);
}


CSemiBoss::~CSemiBoss()
{

}

void CSemiBoss::Tick(double DeltaTime)
{

}

void CSemiBoss::Move()
{

}

void CSemiBoss::Render()
{
	for (int x = 0; x < m_cRect.X; x++)
	{
		for (int y = 0; y < m_cRect.Y; y++)
		{
			//CGraphic::GetInstance()->RenderToBuffer(m_cPosition.X + x, m_cPosition.Y)
		}
	}
}