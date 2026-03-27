// [작성자] : 박재현
// [Desc] : 플레이어 인벤토리

#pragma once
#include "../Standard.h"


class CDummyItem
{
public:
	bool operator==(const CDummyItem& Other)
	{
		return Other.m_sName == m_sName;
	}

public:
	void Use() {}

	// 최대 수량 
	int m_iMaxAmount;

	// 아이템 이름 
	std::string m_sName;

	// 아이템 효과 
};

class CItem;

struct FInventorySlot
{
	FInventorySlot() {}

	void Clear()
	{
		m_pItem = nullptr;	
		m_iOwningAmount = 0;
		m_bEmpty = true;
	}

	void PrintItemData() const
	{
		std::string sMsg = "이름 : " + m_pItem->m_sName + " 보유 수량 : " + std::to_string(m_iOwningAmount) + " 최대 소지 가능 수량 : " + std::to_string(m_pItem->m_iMaxAmount);
		std::cout << sMsg << std::endl;
	}

	bool IsEmpty() const
	{
		return m_bEmpty;
	}

	std::shared_ptr<CDummyItem> m_pItem;
	int m_iOwningAmount;
	bool m_bEmpty = true;
}; 

class CInventory
{
public:
	CInventory();

	CInventory(int iMaxSlot);

	~CInventory();

public:
	// 아이템 사용 함수 
	void Use(int iItemIdx);

	bool CanAddItem(std::shared_ptr<CDummyItem> pItem, int iAmount);

	// 아이템 추가 함수
	void AddItem(std::shared_ptr<CDummyItem> pItem, int iAmount);

	// 인벤토리 내부 아이템 찾는 함수
	void FindItem(const std::string& sItemName);

	// 아이템 제거 (슬롯을 비움)
	void RemoveItem(int iItemIdx, int iRemoveAmount);
	
	// 아이템 정보 출력 함수 
	void PrintItem();

private:
	const int m_ciMaxSlot = 5;

	// 아이템 슬롯
	std::vector<FInventorySlot> m_vItems;
};
