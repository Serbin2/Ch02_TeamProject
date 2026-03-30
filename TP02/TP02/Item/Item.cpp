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
	m_sDesc = "체력 전체 회복";
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

	// float NewHealth = Player->GetMaxHealth(); 
	// Player->SetHealth(NewHealth);
}

CInvincible::CInvincible()
{
	m_sName = "무적 포션";
	m_sDesc = "3초 동안 무적";
	m_iPrice = 800;
	m_iMaxAmount = 1;
	m_dInvinsibleTime = 3.0;
}

void CInvincible::UseItem(std::weak_ptr<CPlayer> pPlayer)
{
	// 유효성 확인
	std::shared_ptr<CPlayer> Player = pPlayer.lock();
	if (!Player)
	{
		return;
	}

	CGraphic::GetInstance()->AddLog("사용! 3초 동안 무적.");

	// Player->SetInvincible(m_dInvinsibleTime);
}
