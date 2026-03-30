#include "Enemy.h"
#include "../Character/Player.h"
#include "../World/GameWorld.h"
#include "../Graphics/ConsoleGraphic.h"
#include <cstdlib>

CEnemy::CEnemy(int Shape, int Color) : CCharacter(Shape, Color)
{
	m_cPosition = { 0, 0 };
	m_fSpeed = 2.0f;
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
	if (!m_bIsValid) return;

	if (m_dMoveTimer > 0.0) m_dMoveTimer -= DeltaTime;
	if (m_dAttackTimer > 0.0) m_dAttackTimer -= DeltaTime;

	if (m_dMoveTimer <= 0.0)
	{
		Move();
		m_dMoveTimer = 1.0 / m_fSpeed;
	}
}

void CEnemy::Move()
{
	int direction = rand() % 4;

	switch (direction)
	{
	case 0: m_cMoveDirection = { 0, -1 }; break;
	case 1: m_cMoveDirection = { 0,  1 }; break;
	case 2: m_cMoveDirection = { -1, 0 }; break;
	case 3: m_cMoveDirection = { 1,  0 }; break;
	}
}

void CEnemy::Attack(COORD Direction)
{
}

void CEnemy::OnHit(float Damage)
{
	if (!m_bIsValid || m_bIsDead) return;

	float finalDamage = Damage - m_fDefense;
	if (finalDamage < 1.0f) finalDamage = 1.0f;

	m_fHealth -= finalDamage;

	if (m_fHealth <= 0.0f)
		Die();
}

void CEnemy::Die()
{
	if (m_bIsDead) return;

	m_fHealth = 0.0f;
	m_bIsDead = true;
	m_bIsValid = false;

	CPlayer* player = CGameWorld::GetInstance()->GetPlayer();
	if (player != nullptr)
	{
		player->AddExp(this);
	}
}