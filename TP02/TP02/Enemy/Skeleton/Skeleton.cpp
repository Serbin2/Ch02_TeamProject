
#include "Skeleton.h"
#include "../../Projectile/Projectile.h"

#define MOVING_TIME 5.0		//	5초동안 움직이기
#define SHOT_DELAY 2.0		//	2초 후에 발사하기
#define MOVE_DELAY 2.0		//	2초 후에 움직이기


CSkeleton::CSkeleton() : CEnemy(Pixel::Skeleton, TEXT_FOREGROUND_BLACK | TEXT_BACKGROUND_WHITE)
{
	m_iState = WaitForMove;
	m_dStateTime = 0;
	m_sName = "스켈레톤";
	//	체력 설정 해야함
}

CSkeleton::~CSkeleton()
{

}

void CSkeleton::Tick(double deltaTime)
{
	if (!m_bIsValid)	return;
	m_dStateTime += deltaTime;

	CEnemy::Tick(deltaTime);
}

void CSkeleton::Move()
{
	switch (m_iState)
	{
	case Moving:
		CEnemy::Move();
		if (m_dStateTime > MOVING_TIME)
		{
			m_iState = WaitForShot;
			m_dStateTime = 0;
		}
		break;
	case WaitForShot:
		if (m_dStateTime > SHOT_DELAY)
		{
			m_iState = WaitForMove;
			m_dStateTime = 0;
			Shot();
		}
		break;
	case WaitForMove:
		if (m_dStateTime > MOVE_DELAY)
		{
			m_iState = Moving;
			m_dStateTime = 0;
		}
	}
}

void CSkeleton::Shot()
{
	if (!CGameWorld::GetInstance()->GetPlayerActor())	return;
	COORD pos = CGameWorld::GetInstance()->GetPlayerActor()->GetPosition();
	COORD dir(0,0);
	int difX = pos.X - m_cPosition.X;
	int difY = pos.Y - m_cPosition.Y;
	
	if (difX != 0)	dir.X = difX > 0 ? 1 : -1;
	if (difY != 0)	dir.Y = difY > 0 ? 1 : -1;

	auto pProjectile = std::make_shared<CProjectile>(Pixel::triangle, TEXT_FOREGROUND_WHITE);
	auto sharedOwner = std::static_pointer_cast<CCharacter>(shared_from_this());
	pProjectile->SetOwner(sharedOwner);
	pProjectile->SetPosition(m_cPosition);
	pProjectile->SetSpeed(5.0f);
	pProjectile->SetMoveDirection(dir);
	CGameWorld::GetInstance()->AddActor(pProjectile);
}