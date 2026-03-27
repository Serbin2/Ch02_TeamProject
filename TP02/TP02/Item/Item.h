// [작성자] : 박다현
// [Desc] : 아이템 베이스 클래스


#pragma once
#include <string>
#include <iostream>
#include <memory>

#include "../Character/Player.h"

// 아이템 인터페이스 클래스 
class cItem abstract
{
protected: 
	std::string m_sName;		// 이름
	std::string m_sDesc;		// 설명
	int m_iPrice;				// 가격
	int m_iMaxAmount;			// 아이템 최대 소지 개수

public:
	cItem(const std::string& sName = "Default", const std::string& sDesc = "default", int iPrice = 0, int iMaxAmount = 0)
		: m_sName(sName), m_sDesc(sDesc), m_iPrice(iPrice), m_iMaxAmount(iMaxAmount)
	{

	}

	virtual ~cItem() = default;

	bool operator==(const cItem& Other)
	{
		return Other.m_sName == m_sName;
	}

	//getter
	const std::string& GetName() const { return m_sName; }
	const std::string& GetDesc() const { return m_sDesc; }
	int GetPrice() const { return m_iPrice; }
	int GetMaxAmount() const { return m_iMaxAmount; }

	//만약을 위한 setter (가격 변경)
	void SetPrice(int p) { if (p >= 0) m_iPrice = p; }

	// 개수 처리
	// void GetItem(int amount = 1) { m_iMaxAmount += amount; } // 아이템획득
	// void ConsumeItem(int amount = 1) { //아이템 사용
	// 	if (m_iMaxAmount >= amount) m_iMaxAmount -= amount;
	// }
	// 
	
	// 아이템 사용시 효과
	virtual void UseItem(std::weak_ptr<class CPlayer> pPlayer) = 0;
};

//예시 체력 회복포션
class Potion : public cItem
{
private:
	int m_iHeal;

public:
	Potion(const std::string & sName = "체력 포션", const std::string & sDesc = "체력을 50 hp 회복한다.",
		int iPrice = 100, int iMaxAmount = 10, int iHeal = 50)
		: cItem(sName, sDesc, iPrice, iMaxAmount)
		, m_iHeal(iHeal)
	{

	}

	void UseItem(std::weak_ptr<class CPlayer> pPlayer) override
	{
		// 유효성 확인
		std::shared_ptr<CPlayer> Player = pPlayer.lock();
		if (!Player)
		{
			return;
		}

		CGraphic::GetInstance()->AddLog("사용! 체력을 " + std::to_string(m_iHeal) + "hp 회복했습니다.");
		Player->SetHealth(Player->GetHealth() + m_iHeal);
	}
};