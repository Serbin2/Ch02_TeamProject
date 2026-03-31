#include "Player.h"
#include "../Input/Input.h"
#include "../Projectile/Projectile.h"
#include "../Inventory/Inventory.h"
#include "../Graphics/Interface.h"
#include "../Graphics/ConsoleGraphic.h"
#include "../Enemy/Enemy.h"
#include <format>


CPlayer::CPlayer(int Shape, int Color) : CCharacter(Shape, Color)
{
	m_cPosition = { 15, 15 };
	m_fSpeed = 3.0f;
	m_dMoveTimer = 0.0;
	m_fInvincibleDuration = 1.0;
	m_dInvincibleTimer = 0.0;
	m_fHealth = 100.0f;
	m_fMaxHealth = 100;
	m_fAttackPower = 115.0f;
	m_fDefense = 5.0f;
	m_iAttackRange = 1;
	m_dAttackCooldown = 0.5;
	m_dAttackTimer = 0.0;
	m_iLevel = 1;
	m_iExp = 0;
	m_iGold = 500;
	m_eTag = ETag::player | ETag::character | ETag::actor;
	m_sName = "[잭]";

	// PJH - 인벤토리 추가
	m_pInventory = std::make_shared<CInventory>();
	m_pProjectile = std::make_shared<CProjectile>();

	CInterface::GetInstance()->AddUI(2, "HP : ");
	string sHP = format("{:.1f}",m_fHealth) + " / " + format("{:.1f}", m_fMaxHealth);
	CInterface::GetInstance()->SetValue(2, sHP);
	CInterface::GetInstance()->AddUI(3, "Lv : ");
	CInterface::GetInstance()->SetValue(3, m_iLevel);
	CInterface::GetInstance()->AddUI(4, "ATF : ");
	CInterface::GetInstance()->AddUI(5, "DEF : ");
	CInterface::GetInstance()->SetValue(4, m_fAttackPower);
	CInterface::GetInstance()->SetValue(5, m_fDefense);
}

void CPlayer::Tick(double DeltaTime)
{
	if (!m_bIsValid)
		return;

	if (m_dMoveTimer > 0.0) m_dMoveTimer -= DeltaTime;
	if (m_dAttackTimer > 0.0) m_dAttackTimer -= DeltaTime;
	if (m_dInvincibleTimer > 0.0) m_dInvincibleTimer -= DeltaTime;

	if (m_bIsInvincibleByItem)
	{
		m_dItemInvincibleTime -= DeltaTime;
		if (m_dItemInvincibleTime <= 0)
		{
			m_dItemInvincibleTime = 0;
			m_bIsInvincibleByItem = false;
		}
	}

	Input();
	Move();
}

void CPlayer::Move()
{
	if ((m_cMoveDirection.X == 0 && m_cMoveDirection.Y == 0) || m_dMoveTimer > 0.0)
		return;

	int nextX = m_cPosition.X + m_cMoveDirection.X;
	int nextY = m_cPosition.Y + m_cMoveDirection.Y;

	// 화면 경계 검사 후 좌표 갱신
	if (nextX >= 0 && nextX < 30 && nextY >= 0 && nextY < 30)
	{
		m_cPosition.X = nextX;
		m_cPosition.Y = nextY;
	}

	m_cMoveDirection = { 0, 0 }; // 이동 후 방향 초기화
	m_dMoveTimer = 1.0 / m_fSpeed;
}

void CPlayer::Attack(COORD Direction)
{
	if (m_dAttackTimer > 0.0)
		return;

	auto pProjectile = m_pProjectile->Clone();
	auto sharedOwner = std::static_pointer_cast<CCharacter>(shared_from_this());
	pProjectile->SetOwner(sharedOwner);
	pProjectile->SetPosition(m_cPosition);
	pProjectile->SetMoveDirection(Direction);
	CGameWorld::GetInstance()->AddActor(pProjectile);
	
	m_dAttackTimer = m_dAttackCooldown;
}

void CPlayer::OnHit(float Damage)
{
	if (m_bIsInvincibleByItem)
		return;

	if (m_dInvincibleTimer > 0.0)
		return;

	float finalDamage = Damage - m_fDefense;
	if (finalDamage > 0.f)
		SetHealth(GetHealth() - finalDamage);
	
	m_dInvincibleTimer = m_fInvincibleDuration;
	string sHP = format("{:.1f}", m_fHealth) + " / " + format("{:.1f}", m_fMaxHealth);
	CInterface::GetInstance()->SetValue(2, sHP);
}

void CPlayer::SetInvincibleStateByItem(double InvincibleTime)
{
	m_bIsInvincibleByItem = true;
	m_dItemInvincibleTime = InvincibleTime;
}

void CPlayer::Input()
{
	CInput* pInput = CInput::GetInstance();
	
	// 콘솔 그래픽은 4사분면((0, 0)의 위치가 좌상단)이므로 y축이 반대로 동작해야합니다.
	// Key Code: https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes

	// Move input
	m_cMoveDirection = { 0, 0 };

	if (pInput->IsKeyPressed('W')) m_cMoveDirection.Y -= 1;
	if (pInput->IsKeyPressed('S')) m_cMoveDirection.Y += 1;
	if (pInput->IsKeyPressed('A')) m_cMoveDirection.X -= 1;
	if (pInput->IsKeyPressed('D')) m_cMoveDirection.X += 1;

	// Attack Input
	if (pInput->IsKeyDown(VK_UP))		  Attack({ 0, -1 }); // [↑]
	else if (pInput->IsKeyDown(VK_DOWN))  Attack({ 0,  1 }); // [↓]
	else if (pInput->IsKeyDown(VK_LEFT))  Attack({ -1, 0 }); // [←]
	else if (pInput->IsKeyDown(VK_RIGHT)) Attack({ 1,  0 }); // [→]

	// PJH - 아이템 키 추가
	std::shared_ptr<CPlayer> pSelf = std::dynamic_pointer_cast<CPlayer>(shared_from_this());
	if (m_pInventory && pSelf)
	{
		if (pInput->IsKeyDown('1')) m_pInventory->UseItem(0, pSelf);
		if (pInput->IsKeyDown('2')) m_pInventory->UseItem(1, pSelf);
		if (pInput->IsKeyDown('3')) m_pInventory->UseItem(2, pSelf);
		if (pInput->IsKeyDown('4')) m_pInventory->UseItem(3, pSelf);
		if (pInput->IsKeyDown('5')) m_pInventory->UseItem(4, pSelf);
	}
}

void CPlayer::AddExp(int exp)
{
	m_iExp += exp;

	while (m_iExp >= m_iLevel * 20)
	{
		m_iExp -= m_iLevel * 20;
		m_iLevel++;

		m_fHealth += 20.0f;
		m_fAttackPower += 5.0f;
		m_fDefense += 2.0f;
		CGraphic::GetInstance()->AddLog("레벨 업!");
	}

	CInterface::GetInstance()->SetValue(4, m_fAttackPower);
	CInterface::GetInstance()->SetValue(5, m_fDefense);
	string sHP = format("{:.1f}", m_fHealth) + " / " + format("{:.1f}", m_fMaxHealth);
	CInterface::GetInstance()->SetValue(2, sHP);
	CInterface::GetInstance()->SetValue(3, (float)m_iLevel);
}