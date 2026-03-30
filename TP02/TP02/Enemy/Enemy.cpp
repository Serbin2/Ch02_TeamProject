#include "Enemy.h"
#include "../Character/Player.h"
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
	m_eTag = ETag::actor | ETag::character | ETag::monster;

}

void CEnemy::Tick(double DeltaTime)
{
	if (!m_bIsValid)
		return;

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
	// 간단한 랜덤 이동 구현
	int direction = rand() % 4;

	switch (direction)
	{
		case 0: m_cMoveDirection = { 0, -1 }; break; // [↑]
		case 1: m_cMoveDirection = { 0,  1 }; break; // [↓]
		case 2: m_cMoveDirection = { -1, 0 }; break; // [←]
		case 3: m_cMoveDirection = { 1,  0 }; break; // [→]
	}
}

void CEnemy::Attack(COORD Direction)
{
	// TODO: 공격 구현 (근접 / 원거리 / 범위 공격 등)
}

void CEnemy::OnHit(float Damage)
{
	// TODO: 피해 계산, 체력 감소, 사망 처리, 피격 효과 등 구현
	if (!m_bIsValid) return;

	float fFinalDamage = Damage - m_fDefense; // 최종 데미지=데미지-방어력;

	if (fFinalDamage < 1.0f)
		fFinalDamage = 1.0f;

	m_fHealth -= fFinalDamage;

	if (m_fHealth <= 0.0f)
	{
		if (player != nullptr)
			CGraphic::GetInstance()->AddLog("exp 획득.");
		Die();
	}
}

void CEnemy::Die()
{
	if (!m_bIsValid) return;
	m_fHealth = 0.0f;
	m_bIsValid = false;
	DropReward();
}

void CEnemy::SetPlayer(CPlayer* InPlayer)
{
	player = InPlayer;
}

void CEnemy::DropReward()
{
	if (player == nullptr) return;

	int dropChance = rand() % 100;

	if (dropChance < 50)
	{
		// 50% 확률 드랍
		// TODO: 아이템 생성 후 월드에 배치
	}
}