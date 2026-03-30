// [작성자] : 박재현
// [Desc] : 플레이어 인벤토리

#pragma once
#include "../Standard.h"
#include "../Item/Item.h"

constexpr INT UI_IVNENTORY__START_LINE = 10;
constexpr INT UI_IVNENTORY__END_LINE = 14;

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

	std::string PrintItemData() const
	{
		std::string sMsg = "이름 : " + m_pItem->GetName() + " 보유 수량 : " + std::to_string(m_iOwningAmount) + " 최대 소지 가능 수량 : " + std::to_string(m_pItem->GetMaxAmount()) + "\n" + m_pItem->GetDesc();
		return sMsg;
	}

	std::string PrintItemDataMinial() const
	{
		std::string sMsg = "Name : " + m_pItem->GetName() + " Amount : " + std::to_string(m_iOwningAmount);
		return sMsg;
	}

	bool IsEmpty() const
	{
		return m_bEmpty;
	}

	std::shared_ptr<cItem> m_pItem;
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
	void UseItem(int iItemIdx, std::weak_ptr<class CPlayer> pPlayer);

	bool CanAddItem(std::shared_ptr<cItem> pItem, int iAmount);

	// 아이템 추가 함수
	bool AddItem(std::shared_ptr<cItem> pItem, int iAmount);

	// 인벤토리 내부 아이템 찾는 함수
	std::vector<int> FindItem(const std::string& sItemName);

	// 아이템 제거 인덱스 기반(슬롯을 비움)
	void RemoveItem(int iItemIdx, int iRemoveAmount);
	
	// 아이템 정보 출력 함수 
	void PrintItem();

	int GetSlotSize() const
	{
		return m_ciMaxSlot;
	}

private:
	const int m_ciMaxSlot = 5;

	// 아이템 슬롯
	std::vector<FInventorySlot> m_vSlots;
};
