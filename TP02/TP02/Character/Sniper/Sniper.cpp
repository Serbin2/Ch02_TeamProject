#include "Sniper.h"
#include "../../Graphics/Interface.h"
#include "../../Input/Input.h"
#include "../../World/GameWorld.h"
#include "../../Graphics/Effect.h"
#include "../../Enemy/Enemy.h"

CSniper::CSniper() : CPlayer(Pixel::Sniper, TEXT_FOREGROUND_BLACK | TEXT_BACKGROUND_MAGENTA)
{
	m_dShotDelay = 3.0;
	m_dInvincibleTime = 2.0;	//	피격시 2초동안 무적,
	m_bInvincible = false;
	m_dMoveTimer = 0.2;

	//	기본 공격 정보를 출력합니다.
	CInterface::GetInstance()->AddUI(5, "ShotDelay ");

	//	디버깅용 위치 정보를 출력합니다.
	CInterface::GetInstance()->AddUI(7, "x ");
	CInterface::GetInstance()->AddUI(8, "y ");

	//	이펙트용 정보를 미리 등록해둡니다.
	m_vShotMaterial.push_back(make_pair(Pixel::circle, TEXT_FOREGROUND_YELLOW | TEXT_BACKGROUND_RED_INT));
	m_vShotMaterial.push_back(make_pair(Pixel::blank, TEXT_FOREGROUND_YELLOW | TEXT_BACKGROUND_RED_INT));
	m_vShotMaterial.push_back(make_pair(Pixel::blank, TEXT_FOREGROUND_YELLOW | TEXT_BACKGROUND_YELLOW));
	m_vShotDuration.push_back(0.1);
	m_vShotDuration.push_back(0.6);
	m_vShotDuration.push_back(0.3);

	m_sName = "스나이퍼";
}

CSniper::~CSniper()
{

}

void CSniper::Tick(double DeltaTime)
{
	if (!m_bIsValid)	return;

	m_dMoveTimer -= DeltaTime;
	m_dShotDelay -= DeltaTime;

	//	기본 공격 쿨타임을 보여줍니다.
	CInterface::GetInstance()->SetValue(5, (float)max(0.0, m_dShotDelay));
	
	//	무적시간 처리입니다.
	if (m_bInvincible)	
	{
		m_dInvincibleTime -= DeltaTime;
		if (m_dInvincibleTime <= 0)
		{
			m_dInvincibleTime = 2.0;
			m_bInvincible = false;
		}
	}

	//	여기서 입력 처리를 함께 했습니다.
	Move();
}

void CSniper::Move()
{
	if (m_dMoveTimer <= 0)
	{	//	움직임 처리입니다.
		if (CInput::GetInstance()->IsKeyPressed('W'))
		{
			m_cPosition.Y -= 1;
			m_dMoveTimer = 0.2;
		}
		if (CInput::GetInstance()->IsKeyPressed('A'))
		{
			m_cPosition.X -= 1;
			m_dMoveTimer = 0.2;
		}
		if (CInput::GetInstance()->IsKeyPressed('S'))
		{
			m_cPosition.Y += 1;
			m_dMoveTimer = 0.2;
		}
		if (CInput::GetInstance()->IsKeyPressed('D'))
		{
			m_cPosition.X += 1;
			m_dMoveTimer = 0.2;
		}

		//	디버깅용 위치 데이터 출력입니다.
		CInterface::GetInstance()->SetValue(7, m_cPosition.X);
		CInterface::GetInstance()->SetValue(8, m_cPosition.Y);
	}

	if (m_dShotDelay <= 0)
	{	//	공격 처리입니다.
		if (CInput::GetInstance()->IsKeyPressed(VK_UP))
		{
			Attack(COORD(0, -1));
			m_dShotDelay = 3.0;
		}
		if (CInput::GetInstance()->IsKeyPressed(VK_LEFT))
		{
			Attack(COORD(-1 , 0));
			m_dShotDelay = 3.0;
		}
		if (CInput::GetInstance()->IsKeyPressed(VK_DOWN))
		{
			Attack(COORD(0, 1));
			m_dShotDelay = 3.0;
		}
		if (CInput::GetInstance()->IsKeyPressed(VK_RIGHT))
		{
			Attack(COORD(1, 0));
			m_dShotDelay = 3.0;
		}
	}
}

void CSniper::Attack(COORD Direction)
{
	//	히트스캔 공격하기
	//	히트스캔으로 찾습니다.
	shared_ptr<CActor> hitActor = CGameWorld::GetInstance()->RayTrace(m_cPosition, Direction, ETag::monster);

	//	타겟 없으면 맵 끝까지 공격
	COORD targetPos(m_cPosition.X + (Direction.X * 30), m_cPosition.Y + (Direction.Y * 30));
	
	if (hitActor != nullptr)
	{
		//	타겟이 있음 -> 데미지 처리 할 것
		targetPos = hitActor.get()->GetPosition();
		targetPos.X -= Direction.X;
		targetPos.Y -= Direction.Y;
		shared_ptr<CEnemy> enemy = dynamic_pointer_cast<CEnemy>(hitActor);
		enemy.get()->OnHit(m_fAttackPower * 10);	//	10배 데미지 주기
		string str = m_sName + "(이)가 " + hitActor.get()->GetName() + "(을)를 공격했습니다!";
		CGraphic::GetInstance()->AddLog(str);
	}

	//	이펙트
	COORD startPos(m_cPosition.X + Direction.X, m_cPosition.Y + Direction.Y);
	shared_ptr<CEffect> FX = make_shared<CEffect>();
	vector<pair<COORD, COORD>>shape;
	shape.push_back(make_pair(startPos, targetPos));
	shape.push_back(make_pair(startPos, targetPos));
	shape.push_back(make_pair(startPos, targetPos));
	FX->CreateDynamicEffect(3, m_vShotMaterial, shape, m_vShotDuration);
	CGameWorld::GetInstance()->AddActor(FX);

	return;
}

void CSniper::OnHit(float Damage)
{
	CPlayer::OnHit(Damage);
	m_bInvincible = true;
}

void CSniper::Render()
{	
	//	무적 시간 관련 처리를 할까 해서 재정의 했으나 처리 추가 안했음
	if (!m_bIsValid)	return;
	CGraphic* pGraphic = CGraphic::GetInstance();
	if (pGraphic)
	{
		pGraphic->RenderToBuffer(m_cPosition.X, m_cPosition.Y, m_pShape, m_tColor);
	}
}