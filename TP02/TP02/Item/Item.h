#pragma once
#include <string>
#include <iostream>

#include "../Character/Player.h"

// 아이템 인터페이스 클래스 
class CItem abstract
{
protected: 
	std::string m_sName;		// 이름
	std::string m_sDesc;		// 설명
	int m_iPrice;				// 가격
	int m_iMaxAmount;			// 아이템 최대 소지 개수

public:
	CItem() = default;

	CItem(const std::string& sName, const std::string& sDesc, int iPrice, int iMaxAmount);


	virtual ~CItem() = default;

	bool operator==(const CItem& Other)
	{
		return Other.m_sName == m_sName;
	}

	//getter
	const std::string& GetName() const { return m_sName; }
	const std::string& GetDesc() const { return m_sDesc; }
	int GetPrice() const { return m_iPrice; }
	int GetMaxAmount() const { return m_iMaxAmount; }

	//만약을 위한 setter (가격 변경)
	void SetPrice(int itemPrice);
	
	// 아이템 사용시 효과
	virtual void UseItem(std::weak_ptr<CPlayer> pPlayer) = 0;

	virtual std::shared_ptr<CItem> Clone() const = 0;
};

//예시 체력 회복포션
class Potion : public CItem
{
private:
	int m_iHeal;

public:
	Potion(const std::string & sName = "체력 포션", const std::string & sDesc = "체력을 50 hp 회복한다.",
		int iPrice = 100, int iMaxAmount = 10, int iHeal = 50)
		: CItem(sName, sDesc, iPrice, iMaxAmount)
		, m_iHeal(iHeal)
	{

	}

	virtual void UseItem(std::weak_ptr<CPlayer> pPlayer)
	{
		// 유효성 확인
		std::shared_ptr<CPlayer> Player = pPlayer.lock();
		if (!Player)
		{
			return;
		}	

		CGraphic::GetInstance()->AddLog("사용! 체력을 " + std::to_string(m_iHeal) + "hp 회복했습니다.");

		float NewHealth = Player->GetHealth() + m_iHeal;
		Player->SetHealth(NewHealth);
	}

	virtual std::shared_ptr<CItem> Clone() const override
	{
		return std::make_shared<Potion>(*this);
	}
};

// 최대 체력으로 설정하는 포션 
class CElixir : public CItem
{
public:
	CElixir();

	virtual void UseItem(std::weak_ptr<CPlayer> pPlayer) override;

	virtual std::shared_ptr<CItem> Clone() const override
	{
		return std::make_shared<CElixir>(*this);
	}
};

// 무적으로 만들어주는 포션 
class CInvinciblePotion : public CItem
{
public:
	CInvinciblePotion();

	virtual void UseItem(std::weak_ptr<CPlayer> pPlayer) override;
	
	virtual std::shared_ptr<CItem> Clone() const override
	{
		return std::make_shared<CInvinciblePotion>(*this);
	}

private:
	double m_dInvinsibleTime;
};

// 스피드 포션 0.1 씩 증가 
class CSpeedPotion : public CItem
{
public:
	CSpeedPotion();

	virtual void UseItem(std::weak_ptr<CPlayer> pPlayer) override;

	virtual std::shared_ptr<CItem> Clone() const override
	{
		return std::make_shared<CSpeedPotion>(*this);
	}

private:
	// 최대체력 증가량
	float m_fSpeedAmount;
};

// 최대 체력 증가 포션 
class CMaxHealthPotion : public CItem
{
public:
	CMaxHealthPotion();

	virtual void UseItem(std::weak_ptr<CPlayer> pPlayer) override;

	virtual std::shared_ptr<CItem> Clone() const override
	{
		return std::make_shared<CMaxHealthPotion>(*this);
	}

private:
	float m_fMaxHealthAmount;
};

//드롭 아이템들
class CScrap : public CItem
{
public:
	CScrap();

	virtual void UseItem(std::weak_ptr<CPlayer> pPlayer) override;

	virtual std::shared_ptr<CItem> Clone() const override
	{
		return std::make_shared<CScrap>(*this);
	}
};

