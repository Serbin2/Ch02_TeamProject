#include "Item.h"
#include <iostream>

CItem::CItem(const std::string& sName, const std::string& sDesc, int iPrice, int iMaxAmount)
	: m_sName(sName), m_sDesc(sDesc), m_iPrice(iPrice), m_iMaxAmount(iMaxAmount)
{

}

void CItem::SetPrice(int itemPrice)
{
	if (itemPrice >= 0) m_iPrice = itemPrice;
}

CElixir::CElixir()
{
	m_sName = "엘릭서";
	m_sDesc = "최대 체력으로 회복한다.";
	m_iPrice = 500;
	m_iMaxAmount = 3;
}

void CElixir::UseItem(std::weak_ptr<CPlayer> pPlayer)
{
	// 유효성 확인
	std::shared_ptr<CPlayer> Player = pPlayer.lock();
	if (!Player)
	{
		return;
	}

	CGraphic::GetInstance()->AddLog("사용! 최대 체력.");

	float NewHealth = Player->GetMaxHealth(); 
	Player->SetHealth(NewHealth);
}

CInvinciblePotion::CInvinciblePotion()
{
	m_sName = "무적 포션";
	m_sDesc = "3초 동안 무적상태가 된다.";
	m_iPrice = 800;
	m_iMaxAmount = 3;
	m_dInvinsibleTime = 3.0;
}

void CInvinciblePotion::UseItem(std::weak_ptr<CPlayer> pPlayer)
{
	// 유효성 확인
	std::shared_ptr<CPlayer> Player = pPlayer.lock();
	if (!Player)
	{
		return;
	}

	CGraphic::GetInstance()->AddLog("사용! 3초 동안 무적.");
	Player->SetInvincibleStateByItem(m_dInvinsibleTime);
}

CSpeedPotion::CSpeedPotion()
{
	m_sName = "스피드 포션";
	m_sDesc = "스피드가 0.1 증가한다.";
	m_iPrice = 200;
	m_iMaxAmount = 3;
	m_fSpeedAmount = 0.1f;
}

void CSpeedPotion::UseItem(std::weak_ptr<CPlayer> pPlayer)
{
	// 유효성 확인
	std::shared_ptr<CPlayer> Player = pPlayer.lock();
	if (!Player)
	{
		return;
	}

	CGraphic::GetInstance()->AddLog("사용! 3초 동안 무적.");
	float NewSpeed = Player->GetSpeed() + m_fSpeedAmount;
	Player->SetSpeed(NewSpeed);
}

CMaxHealthPotion::CMaxHealthPotion()
{
	m_sName = "최체 포션";
	m_sDesc = "최대체력이 20 증가한다";
	m_iPrice = 200;
	m_iMaxAmount = 3;
	m_fMaxHealthAmount = 20.f;
}

void CMaxHealthPotion::UseItem(std::weak_ptr<CPlayer> pPlayer)
{
	// 유효성 확인
	std::shared_ptr<CPlayer> Player = pPlayer.lock();
	if (!Player)
	{
		return;
	}

	CGraphic::GetInstance()->AddLog("사용! 최대 체력 20 증가.");
	float NewMaxHealth = Player->GetMaxHealth() + m_fMaxHealthAmount;
	Player->SetMaxHealth(NewMaxHealth);
}

/// 아이템 드롭
CScrap::CScrap()
{
	m_sName = "고철";
	m_sDesc = "쓸모는 애매한데 왠지 버리면 손해 같아서 들고 가게 된다.";
	m_iPrice = 50; // 30gold 
	m_iMaxAmount = 99;
}

void CScrap::UseItem(std::weak_ptr<CPlayer> pPlayer)
{
	CGraphic::GetInstance()->AddLog("상점 판매용.");
	return;
}

