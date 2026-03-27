#include "Player.h"
#include "../Input/Input.h"
#include "../Projectile/Projectile.h"

CPlayer::CPlayer(int Shape, int Color) : CCharacter(Shape, Color)
{
	m_cPosition = { 15, 15 };
	m_fSpeed = 3.0f;
	m_fHealth = 100.0f;
	m_fAttackPower = 15.0f;
	m_fDefense = 5.0f;
	m_iAttackRange = 1;
	m_dAttackCooldown = 0.5;
	m_dAttackTimer = 0.0;
	m_iLevel = 1;
	m_iExp = 0;
	m_eTag = ETag::player | ETag::character | ETag::actor;
}

void CPlayer::Tick(double DeltaTime)
{
	if (!m_bIsValid)
		return;

	if (m_dMoveTimer > 0.0) m_dMoveTimer -= DeltaTime;
	if (m_dAttackTimer > 0.0) m_dAttackTimer -= DeltaTime;

	Input();

	if ((m_cMoveDirection.X != 0 || m_cMoveDirection.Y != 0) && m_dMoveTimer <= 0.0)
	{
		Move();
		m_dMoveTimer = 1.0 / m_fSpeed;
	}
	else
	{
		m_cMoveDirection = { 0, 0 };
	}

	Render();
}

void CPlayer::Move()
{
	int nextX = m_cPosition.X + m_cMoveDirection.X;
	int nextY = m_cPosition.Y + m_cMoveDirection.Y;

	if (nextX >= 0 && nextX < 30 && nextY >= 0 && nextY < 30)
	{
		m_cPosition.X = nextX;
		m_cPosition.Y = nextY;
	}

	m_cMoveDirection = { 0, 0 }; // 이동 후 방향 초기화
}

void CPlayer::Attack(COORD Direction)
{
	if (m_dAttackTimer > 0.0) // 공격 쿨타임이 남아있으면 공격하지 않음
		return;

	auto pProjectile = std::make_shared<CProjectile>(Pixel::circle, TEXT_FOREGROUND_YELLOW);
	auto sharedOwner = std::static_pointer_cast<CCharacter>(shared_from_this());
	pProjectile->SetOwner(sharedOwner);
	pProjectile->SetPosition(m_cPosition);
	pProjectile->SetSpeed(5.0f);
	pProjectile->SetMoveDirection(Direction);
	CGameWorld::GetInstance()->AddActor(pProjectile);

	m_dAttackTimer = m_dAttackCooldown; // 공격 후 쿨타임 초기화
}

void CPlayer::OnHit(float Damage)
{
	// TODO: 피해 계산, 체력 감소, 사망 처리, 피격 효과 등 구현
}

void CPlayer::Input()
{
	CInput* pInput = CInput::GetInstance();
	
	// 콘솔 그래픽은 4사분면((0, 0)의 위치가 좌상단)이므로 y축이 반대로 동작해야합니다.
	// Key Code: https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes

	// Move input
	if (pInput->IsKeyPressed('W')) m_cMoveDirection = { 0, -1 };
	if (pInput->IsKeyPressed('S')) m_cMoveDirection = { 0,  1 };
	if (pInput->IsKeyPressed('A')) m_cMoveDirection = { -1, 0 };
	if (pInput->IsKeyPressed('D')) m_cMoveDirection = { 1,  0 };

	// Attack input
	if (pInput->IsKeyDown(VK_UP))		  Attack({ 0, -1 }); // [↑]
	else if (pInput->IsKeyDown(VK_DOWN))  Attack({ 0,  1 }); // [↓]
	else if (pInput->IsKeyDown(VK_LEFT))  Attack({ -1, 0 }); // [←]
	else if (pInput->IsKeyDown(VK_RIGHT)) Attack({ 1,  0 }); // [→]
}
