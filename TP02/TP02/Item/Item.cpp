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

/// 아이템 드롭
CSlimeJelly::CSlimeJelly()
{
	m_sName = "고철덩어리";
	m_sDesc = "쓸모는 애매한데 왠지 버리면 손해 같아서 들고 가게 된다.";
	m_iPrice = 50;
	m_iMaxAmount = 99;
}

void CSlimeJelly::UseItem(std::weak_ptr<CPlayer> pPlayer)
{
	CGraphic::GetInstance()->AddLog("상점 판매용.");
}

CBonePiece::CBonePiece()
{
	m_sName = "보안 칩";
	m_sDesc = "00사의 차세대 경비 시스템 핵심 부품. 홍보 문구는 화려했지만, \n 지금은 주머니 속 잡템 신세다.";
	m_iPrice = 70;
	m_iMaxAmount = 99;
}
void CBonePiece::UseItem(std::weak_ptr<CPlayer> pPlayer)
{
	CGraphic::GetInstance()->AddLog("상점 판매용.");
}

CStoneFragment::CStoneFragment()
{
	m_sName = "고철 코어";
	m_sDesc = "폐기 처리장 여러 곳의 잔해를 끌어모아 만든 압축 동력 코어.\n 출력은 아직 살아 있지만, 왜 이런 게 움직였는지는 아무도 설명하지 못한다.";
	m_iPrice = 90;
	m_iMaxAmount = 99;
}
void CStoneFragment::UseItem(std::weak_ptr<CPlayer> pPlayer)
{
	CGraphic::GetInstance()->AddLog("상점 판매용.");
}
