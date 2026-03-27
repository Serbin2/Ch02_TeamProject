#include "Inventory.h"
#include "../Item/Item.h"
#include "../Utils/Utils.h"

#include "../Graphics/ConsoleGraphic.h"

CInventory::CInventory()
{
	m_vItems.resize(m_ciMaxSlot);
}

CInventory::CInventory(int iMaxSlot)
{
	m_vItems.resize(iMaxSlot);
}

bool CInventory::CanAddItem(std::shared_ptr<CDummyItem> pItem, int iAmount)
{
	int Remaining = iAmount;

	for (const auto& Slot : m_vItems)
	{
		// 아이템 겹처서 보관가능한지 판단
		if (Slot.m_pItem && Slot.m_pItem->m_sName == pItem->m_sName)
		{
			int CanStack = Slot.m_pItem->m_iMaxAmount - Slot.m_iOwningAmount;

			Remaining -= CanStack;

			if (Remaining <= 0)
				return true;
		}
	}

	// 빈 슬롯 확인
	for (const auto& Slot : m_vItems)
	{
		if (Slot.m_pItem == nullptr)
		{
			Remaining -= pItem->m_iMaxAmount;

			if (Remaining <= 0)
				return true;
		}
	}

	return false;
}

void CInventory::Use(int iItemIdx)
{
	int iSize = static_cast<int>(m_vItems.size());

	// 잘못된 인덱스 접근
	if (iItemIdx < 0 ||  iSize <= iItemIdx)
	{
		return;
	}
	
	FInventorySlot& Slot = m_vItems.at(iItemIdx);
	if (Slot.m_iOwningAmount >= 1)
	{
		Slot.m_pItem->Use();
		RemoveItem(iItemIdx, 1);
	}
}

void CInventory::AddItem(std::shared_ptr<CDummyItem> pItem, int iAmount)
{
	if (!CanAddItem(pItem, iAmount))
	{
		CGraphic::GetInstance()->AddLog("아이템을 더 이상 추가할수 없음");
		return;
	}

	int Remaining = iAmount;

	// 기존 슬롯 채우기
	for (auto& Slot : m_vItems)
	{
		// 같은 이름의 아이템 찾기
		if (Slot.m_pItem && Slot.m_pItem->m_sName == pItem->m_sName)
		{
			// 현재 슬롯에 추가로 넣을수 있는 아이템 개수
			int CanAddAmount = Slot.m_pItem->m_iMaxAmount - Slot.m_iOwningAmount;

			// 더 작은값을 현재 슬롯에 넣음
			int AddAmount = min(Remaining, CanAddAmount);

			// 실제 슬롯 아이템 개수 증가
			Slot.m_iOwningAmount += AddAmount;

			// 남은 아이템 개수 감소
			Remaining -= AddAmount;

			// 남은 아이템 개수가 0보다 작거나 같으면, 존재하는 슬롯에 아이템을 다 넣은 상태
			if (Remaining <= 0)
				return;
		}
	}

	// 위에서 모든 슬롯
	for (auto& Slot : m_vItems)
	{
		if (Slot.IsEmpty())
		{
			// 아이템 클래스 넣기 
			Slot.m_pItem = pItem;

			// 더 작은값을 현재 슬롯에 넣음
			int AddAmount = min(Remaining, pItem->m_iMaxAmount);

			// 현재 슬롯 개수 설정 
			Slot.m_iOwningAmount = AddAmount;

			// 남은 아이템 감소
			Remaining -= AddAmount;

			// 0보다 작으면 모두 수납완료
			if (Remaining <= 0)
				return;
		}
	}
}

void CInventory::FindItem(const std::string& sItemName)
{
	for (int i = 0; i < m_vItems.size(); i++)
	{
		const FInventorySlot& Slot = m_vItems[i];

		if (Slot.m_pItem && Slot.m_pItem->m_sName == sItemName)
		{
			std::cout << "아이템 발견 : 슬롯 " << i << std::endl;
			Slot.PrintItemData();
			return;
		}
	}
}

void CInventory::RemoveItem(int iItemIdx, int iRemoveAmount)
{
	FInventorySlot& Slot = m_vItems.at(iItemIdx);

	Slot.m_iOwningAmount -= iRemoveAmount;
	if (Slot.m_iOwningAmount <= 0)
	{
		Slot.Clear();
	}
}

void CInventory::PrintItem()
{
	std::cout << "===== Inventory =====\n";

	for (int i = 0; i < m_vItems.size(); i++)
	{
		std::cout << "[" << i << "] ";

		const FInventorySlot& Slot = m_vItems[i];

		if (Slot.m_pItem == nullptr)
		{
			std::cout << "Empty\n";
		}
		else
		{
			Slot.PrintItemData();
		}
	}
}
