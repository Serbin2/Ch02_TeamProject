#include "Enemy.h"
#include "../Character/Player.h"
#include "../World/GameWorld.h"
#include "../Graphics/ConsoleGraphic.h"
#include <format>


CEnemy::CEnemy(int Shape, int Color) : CCharacter(Shape, Color)
{
	m_cPosition = { 0, 0 };
	m_fSpeed = 1.0f;
	m_dMoveTimer = 0.0;
	m_fInvincibleDuration = 0.5;
	m_dInvincibleTimer = 0.0;
	m_fHealth = 50.0f;
	m_fAttackPower = 10.0f;
	m_fDefense = 3.0f;
	m_iAttackRange = 1;
	m_dAttackCooldown = 1.0;
	m_dAttackTimer = 0.0;
	m_iExpReward = 50;
	m_bIsDead = false;
	m_eTag = ETag::actor | ETag::character | ETag::monster;
}

void CEnemy::Tick(double DeltaTime)
{
	if (!m_bIsValid)
		return;

	if (m_dMoveTimer > 0.0) m_dMoveTimer -= DeltaTime;
	if (m_dInvincibleTimer > 0.0) m_dInvincibleTimer -= DeltaTime;

	Move();
	CheckCollision();
}

void CEnemy::Move()
{
	if (m_dMoveTimer > 0.0)
		return;

	auto pPlayer = CGameWorld::GetInstance()->GetPlayerActor();
	if (pPlayer == nullptr)
		return;

	int diffX = pPlayer->GetPosition().X - m_cPosition.X;
	int diffY = pPlayer->GetPosition().Y - m_cPosition.Y;

	m_cMoveDirection = { 0, 0 };

	if (diffX != 0)
		m_cMoveDirection.X = (diffX > 0) ? 1 : -1;
	if (diffY != 0)
		m_cMoveDirection.Y = (diffY > 0) ? 1 : -1;

	int nextX = m_cPosition.X + m_cMoveDirection.X;
	int nextY = m_cPosition.Y + m_cMoveDirection.Y;

	// 화면 경계 검사 후 좌표 갱신
	if (nextX >= 0 && nextX < 30 && nextY >= 0 && nextY < 30)
	{
		m_cPosition.X = nextX;
		m_cPosition.Y = nextY;
	}

	m_cMoveDirection = { 0, 0 };
	m_dMoveTimer = 1.0 / m_fSpeed;
}

void CEnemy::CheckCollision()
{
	auto pPlayer = CGameWorld::GetInstance()->GetPlayerActor();
	if (pPlayer == nullptr || !pPlayer->IsValid())
		return;

	COORD playerPosition = pPlayer->GetPosition();
	if (m_cPosition.X == playerPosition.X && m_cPosition.Y == playerPosition.Y) // 플레이어와 겹친 경우 데미지 부여
		pPlayer->OnHit(m_fAttackPower);
}

void CEnemy::OnHit(float Damage)
{
	if (m_dInvincibleTimer > 0.0)
		return;

	float finalDamage = Damage - m_fDefense;
	m_fHealth -= finalDamage > 0 ? finalDamage : 0; // 방어력이 공격력보다 높으면 피해는 0으로 처리
	m_dInvincibleTimer = m_fInvincibleDuration;

	if (m_fHealth <= 0)
	{
		m_bIsValid = false;
		//string str = m_sName + "이(가) 처치되었습니다!";
		CGraphic::GetInstance()->AddLog(m_sName + "이(가) 처치되었습니다!");
		Die();
	}
}

void CEnemy::Die()
{
	auto pPlayer = CGameWorld::GetInstance()->GetPlayerActor();
	if (pPlayer == nullptr || !pPlayer->IsValid())	return;
	
	dynamic_pointer_cast<CPlayer>(pPlayer)->AddExp(m_iExpReward);

	
}
